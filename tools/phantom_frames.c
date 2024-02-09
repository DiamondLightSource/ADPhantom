/** 
 * This program demonstrates how to capture packets using pcap and parse these as Phantom camera frames.
 * 
 * Build the application with gcc like this:  gcc -o phantom-frames phantom_frames.c -lpcap
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap/pcap.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>

#ifndef ETHERTYPE_IEEE802A
#define ETHERTYPE_IEEE802A 0x88B7
#endif

typedef struct {
    uint32_t packet_counter;
    uint32_t frame_counter;
    uint32_t frame_packet_counter;
    uint32_t frame_bytes_counter;
    u_char *frame_data;  // raw, unscrambled, tightly packed pixels.
} frame_data_type;

void frame_handler(const frame_data_type *frame);
void unpack_frame_P10(const u_char * src, uint32_t src_len, uint16_t *dst, uint32_t dst_len);
void save_frame_P10(const u_char * src, uint32_t src_len, const char *filename);
void packet_handler(
    u_char *args,
    const struct pcap_pkthdr *header,
    const u_char *packet
);
void ip_packet_handler(
    u_char *args,
    const struct pcap_pkthdr *header,
    const u_char *packet
);
void ieee802a_packet_handler(
    u_char *args,
    const struct pcap_pkthdr *header,
    const u_char *packet
);

/* Callback packet handler function */
void packet_handler(
    u_char *args,
    const struct pcap_pkthdr *header,
    const u_char *packet
)
{
    /* The total packet length, including all headers
       and the data payload is stored in
       header->len and header->caplen. Caplen is
       the amount actually available, and len is the
       total packet length even if it is larger
       than what we currently have captured. If the snapshot
       length set with pcap_open_live() is too small, you may
       not have the whole packet. */
    // printf("Total packet size available: %d bytes\n", header->caplen);
    // printf("Expected packet size: %d bytes\n", header->len);

    /* First, lets make sure we have an IEEE802a packet */
    struct ether_header *eth_header;
    eth_header = (struct ether_header *) packet;

    if (ntohs(eth_header->ether_type) == ETHERTYPE_IP) {
        ip_packet_handler(args, header, packet);
    } else if (ntohs(eth_header->ether_type) == ETHERTYPE_IEEE802A) {
        ieee802a_packet_handler(args, header, packet);
    } else {
        printf("Unsupported ethernet packet type: 0x%04X\n", eth_header->ether_type);
    }
}

void ip_packet_handler(
    u_char *args,
    const struct pcap_pkthdr *header,
    const u_char *packet
)
{
    struct ether_header *eth_header;
    eth_header = (struct ether_header *) packet;

    /* Pointers to start point of various headers */
    const u_char *ip_header;
    const u_char *tcp_header;
    const u_char *payload;

    /* Header lengths in bytes */
    int ethernet_header_length = 14; /* Doesn't change */
    int ip_header_length;
    int tcp_header_length;
    int payload_length;

    /* Find start of IP header */
    ip_header = packet + ethernet_header_length;
    /* The second-half of the first byte in ip_header
       contains the IP header length (IHL). */
    ip_header_length = ((*ip_header) & 0x0F);
    /* The IHL is number of 32-bit segments. Multiply
       by four to get a byte count for pointer arithmetic */
    ip_header_length = ip_header_length * 4;

    /* Detect the protocol: its always the 10th byte of the IP header */
    u_char protocol = *(ip_header + 9);
    if (protocol == IPPROTO_UDP) {
        printf("UDP packet\n");
    } else if (protocol != IPPROTO_TCP) {
        printf("Not a TCP packet. Skipping...\n\n");
        return;
    }

    /* Add the ethernet and ip header length to the start of the packet
       to find the beginning of the TCP header */
    tcp_header = packet + ethernet_header_length + ip_header_length;
    tcp_header_length = ((*(tcp_header + 12)) & 0xF0) >> 4;
    tcp_header_length = tcp_header_length * 4;

    /* Add up all the header sizes to find the payload offset */
    int total_headers_size = ethernet_header_length+ip_header_length+tcp_header_length;
    payload_length = header->caplen -
        (ethernet_header_length + ip_header_length + tcp_header_length);
    payload = packet + total_headers_size;

    /* Print payload in ASCII */
    if (payload_length > 0) {
        printf("TCP packet: payload size: %d bytes. Data:\n", payload_length);
        const u_char *temp_pointer = payload;
        int byte_count = 0;
        while (byte_count++ < payload_length) {
            printf("%c", *temp_pointer);
            temp_pointer++;
        }
        printf("\n");
    }
    return;
}

typedef struct __attribute__((__packed__)){
        u_char OUI_1;
        u_char OUI_2;
        u_char OUI_3;
        u_short protocol_id;
    } ether_extended_header_type;

/* What are the 13 bytes of header in the packet payload??? */
typedef struct __attribute__((__packed__)){
        u_int32_t counter;  // Assuming this is a counter as it increments. But what is it counting?
        u_char byte_5;
        u_char byte_6;
        u_char byte_7;
        u_char byte_8;
        u_char byte_9;
        u_int32_t frame_size_bytes;  // the last four bytes are constant in the packets: 00 13 88 00 = 128000 which happens to match the framesize detected
    } phantom_packet_header_type;

void ieee802a_packet_handler(u_char *args, const struct pcap_pkthdr *header,
                             const u_char *packet)
{
    static uint32_t last_frame_header_counter = 0;
    uint32_t diff_last_frame = 0;
    const int ethernet_header_length = 14;
    const int ethernet_extended_header_length = 5;
    const int phantom_packet_header_length = 13;
    int payload_length;
    phantom_packet_header_type *phantom_packet_header;
    const u_char* payload;
    frame_data_type* frame_data = (frame_data_type*)args;
    frame_data->packet_counter++;
    // printf("Packet number: %d\n", frame_data->packet_counter);

    ether_extended_header_type *ether_extended_header = (ether_extended_header_type *)(packet + ethernet_header_length);
    
    phantom_packet_header = (phantom_packet_header_type *)(packet + ethernet_header_length + ethernet_extended_header_length);
    phantom_packet_header->frame_size_bytes = ntohl(phantom_packet_header->frame_size_bytes);
    phantom_packet_header->counter = ntohl(phantom_packet_header->counter) >> 8;
    diff_last_frame = phantom_packet_header->counter - last_frame_header_counter;
    last_frame_header_counter = phantom_packet_header->counter;
    printf("Counter: %010u 0x%08X - diff: %d - PID=%d\n", 
            phantom_packet_header->counter,
            phantom_packet_header->counter,
            diff_last_frame,
            0x00FF & ether_extended_header->protocol_id);

    payload_length = header->caplen - (ethernet_header_length + ethernet_extended_header_length + phantom_packet_header_length);
    payload = packet + ethernet_header_length + ethernet_extended_header_length + phantom_packet_header_length;
    // printf("Payload size: %d bytes\n", payload_length);

    frame_data->frame_packet_counter++;

    if (frame_data->frame_packet_counter == 1) {
        printf(
            "Detected first packet in frame (frame#:%d packets#:%d)\n", 
            frame_data->frame_counter, frame_data->packet_counter);
            // TODO: if we want to actually do anything with the frame data we should move the frame_data->frame_data pointer here
    }

    // Copy the payload data into the frame buffer - note this is encoded, tightly packed
    // This memcpy is necessary as the packet payload is only available for the duration of this callback.
    // Copying small chunks like this (1472 bytes) x 870 packets is inefficient so we might need to benchmark this to understand how bad it is.
    memcpy(frame_data->frame_data + frame_data->frame_bytes_counter, payload, payload_length);
    frame_data->frame_bytes_counter += payload_length;

    // TODO: this is a brittle way of detecting the end of a frame.
    //       Probably better to count bytes in packets - or even better use the frame header counter if possible?
    if (payload_length < 1472) {
        printf(" Detected last packet in frame (no.:%d bytes:%d packets:%d)\n", 
                frame_data->frame_counter, frame_data->frame_bytes_counter, frame_data->frame_packet_counter);
        frame_handler(frame_data);
        frame_data->frame_counter++;
        frame_data->frame_packet_counter = 0;
        frame_data->frame_bytes_counter = 0;
    }
}

/** Called whenever we have a full frame assembled. */
void frame_handler(const frame_data_type *frame)
{
    printf(" Frame size: %d bytes\n", frame->frame_bytes_counter);
    printf(" Frame packet count: %d\n", frame->frame_packet_counter);
    printf(" Frame number: %d\n", frame->frame_counter);
}

// ***
// ** Save a 1280 x 800 greyscale frame to a PGM formatted image file
// *  only use with decoded image data.
void save_frame_P10(const u_char * decoded_frame, uint32_t frame_bytes, const char *filename)
{
        FILE *pgmimg = fopen(filename, "wb");
        fprintf(pgmimg, "P5\n%d %d\n1023\n", 1280, 800);
        fwrite(decoded_frame, sizeof(u_char), frame_bytes, pgmimg);
        fclose(pgmimg);
        printf("Saved frame to frame.pgm\n");
}

static inline uint32_t byteswap32(const uint32_t x)
{
    return (((x & 0x000000FF) << 24) |
            ((x & 0x0000FF00) <<  8) |
            ((x & 0x00FF0000) >>  8) |
            ((x & 0xFF000000) >> 24));
}
static inline uint16_t byteswap16(const uint16_t x)
{
    return (((x & 0x00FF) << 8) |
            ((x & 0xFF00) >> 8));
}

/** Unpack an image frame with the P10 formatting
 * 
 * The P10 formatting is explained as: 10 bits per pixel packed into 32-bit big-endian words.
 * Except that's a lie. The 10 bits per pixel are actually packed into 40 bits, so that's 5 bytes per pixel.
 * 
 * WARNING: this method does not currently appear to produce a correct result!
 */
void unpack_frame_P10(const u_char * src, uint32_t src_len, uint16_t *dst, uint32_t dst_len)
{
    uint16_t swapped_word_16;
    uint16_t src_word_16;
    if (dst_len < src_len*4/5)
    {
        printf("Error: destination buffer too small. Should be at least %bytes\n", src_len*4/5);
        return;
    }
    // for each 40bit/5byte chunk, extract 4 x 10bit pixels
    for (uint32_t i = 0; i < src_len; i+=5)
    {
        for (uint16_t j = 0; j < 4; j++){
            src_word_16 = *(uint16_t*)(src + i + j);
            swapped_word_16 = byteswap16(src_word_16);
            *dst = swapped_word_16 & 0x03FF;
            dst++;
        }
    }
}

int main(int argc, char **argv) {    
    char error_buffer[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    /* Snapshot length is how many bytes to capture from each packet. */
    int snapshot_length = 1504;
    /* End the loop after this many packets are captured */
    // int total_packet_count = 8712;
    int total_packet_count = 0;
    u_char *my_arguments = NULL;

    // Swap the comments on these two lines to swap between reading file or ethernet port
    // handle = pcap_open_live("eth0", snapshot_length, 0, 10000, error_buffer);
    handle = pcap_open_offline("test_2_tcp_10g.pcap", error_buffer);

    if (handle == NULL) {
        printf("Could not open device or file: %s\n", error_buffer);
        return 2;
    }

    const uint32_t max_frame_size_bytes = 1280 * 800 * 2;
    frame_data_type *frame_data_type_ptr = (frame_data_type *)calloc(1, sizeof(frame_data_type));
    frame_data_type_ptr->frame_data = (u_char *)calloc(sizeof(u_char), max_frame_size_bytes);

    pcap_loop(handle, total_packet_count, packet_handler, (u_char *)frame_data_type_ptr);
    pcap_close(handle);
    return 0;
}