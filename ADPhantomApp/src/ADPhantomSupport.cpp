#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <ADPhantom.h>
using namespace std;

// '<' and '==' operator functions for use with phantomVal type map
bool operator<(const phantomVal& m1, const phantomVal& m2)
{
  return (m1.value < m2.value) ? true : false;
}
bool operator==(const phantomVal& m1, const phantomVal& m2)
{
  return (m1.value == m2.value) ? true : false;
}

std::string stripControl(std::string& str, const char* ws)
// Remove "control" characters (defined in ws) from the input string
{
  string::iterator it;
  int wspos = 0;
  for (it=str.begin(); it!=str.end(); ++it) {
        wspos = str.find_first_of(ws, wspos);
        if (wspos < 0) break; 
        str.erase(wspos, 1);
  }
  return str;
}

std::string deleteParen(std::string& str)
// Remove all "{}" characters from the input string
{
  std::string newstring;
  for (size_t i=0; i<str.length(); ++i)
  {
    if ((str.at(i) != '{') && (str.at(i) != '}')) {
      newstring.push_back(str.at(i));
    }
  }
  return newstring;
}

// trim whitespace from left
inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim whitespace from right
inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v")
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// trim whitespace from left & right
inline std::string& trim(std::string& s, const char* t = " \t\n\r\f\v")
{
    return ltrim(rtrim(s, t), t);
}

std::string stripChar(std::string& str, const char c)
// Remove all occurences of a single character from the input string
{
  std::string newstring;
  for (size_t i=0; i<str.length(); ++i)
  {
    if (str.at(i) != c) {
      newstring.push_back(str.at(i));
    }
  }
  return newstring;
}

bool repeatTerm(const std::string instring)
{
/* Check whether the input string contains just a pair of repeated terminators,
   ( '}' or ',' ), having removed any whitespace first */
  string::iterator it;
  string str;
  char ws[] = " \t\n\r\f\v";
  int wspos = 0;
// Remove whitespace  
  str = instring;
  for (it=str.begin(); it!=str.end(); ++it) {
        wspos = str.find_first_of(ws, wspos);
        if (wspos < 0) break; 
        str.erase(wspos, 1);
  }
  if (str.size() == 2)
    return (str == "}}" || str == "}," || str == ",}");
  else
    return false;
}

PHANTOMValueType_t phantomType(const string str)
// Returns the type of the value, determined by examining the contents
// The recognised types are defined in the camera protocol document
{
  char cstring[256];
  int i;
  string tstring = str;
  string hstring = trim(tstring);
  
// assume string type if contains quotation mark
  if (hstring.find('"') != std::string::npos) {
      return PHANTOMTypeString;
    }
// Flag list contains both '{' and '}'  
  if ( (hstring.find('{') != std::string::npos) &&
       (hstring.find('}') != std::string::npos) )
      return PHANTOMTypeFlags;
    
   stripControl(hstring, " \t\n\r\f\v"); // Can now remove embedded spaces etc.

// Hex number starts with "0x" and subsequently only Hex digits
  if (hstring.substr(0,2) == "0x") {
    string testhex = hstring.substr(2);
    char hxstring[256];
    strncpy(hxstring, testhex.c_str(), 256);
    if (strspn(hxstring, "1234567890abcdefABCDEF") == strlen(hxstring))
     return PHANTOMTypeHex;
  }

  strncpy(cstring, hstring.c_str(), 256);  // string as C char array
// Resolution type is of form 123x456; containing only 'x', space and decimal digits
  if ( (sscanf(cstring, "%ux%u", &i, &i) == 2) &&
       (strspn(cstring, " 1234567890x") == strlen(cstring)) )
    return PHANTOMTypeRes;
// Integer contains only decimal digits and sign
  if (strspn(cstring, "+-1234567890") == strlen(cstring)) return PHANTOMTypeInteger;
// Float must contain a '.' and at least 1 digit; only allowed decimal digits and sign
  if ((hstring.find('.') != std::string::npos) && 
      (strspn(cstring, "+-1234567890.") == strlen(cstring)) &&
      (hstring.size() > 1) )
     return PHANTOMTypeFloat;
// nothing matched  
  return PHANTOMTypeUnknown;
}

void addDataItem(vector<string>& strucName, vector<phantomVal>& value, const string itemname, const string valstring)
{
// Add new vector item: item name and value string/type  
  phantomVal mv;
  string s, i, v;
  PHANTOMValueType_t mt;

  i = itemname;
  strucName.push_back(stripChar(i, ' '));  // Remove spaces in name when adding item
  v = valstring;
  s = ltrim(v);
  mt = phantomType(s);
  if (mt == PHANTOMTypeString) s = stripChar(s, '"');  // Strip quotes from strings
  mv.setValue(s);          // Set string value and type in phantomVal object
  mv.setType(mt);
  value.push_back(mv);     // Add phantomVal object as new vector item
}


void parseDataStruc(const string& str, vector<string>& strucName, vector<phantomVal>& value)
{
  string::iterator it;
  string valstring, instring, itemname;
  string strucPrefix[10];
  int oppos, cppos, last_cppos, colpos, nestlev;

  instring = str;
  last_cppos = colpos = nestlev = 0;
  cppos = oppos = -1;
  for (unsigned int j = 0; j<instring.size(); j++) {
    if (instring[j] == '{') {
      oppos = j;                                          // Opening parenthesis position
      nestlev++;                                          // Increment nest level at '{'
    }
    else if (instring[j] == '}' || instring[j] == ',') {
      cppos = j;                                         // Closing parenthesis/comma position
      valstring = instring.substr(colpos+1, j-colpos-1); // Item value follows colon
      itemname = "";
      for (int k = 0; k<= nestlev; k++ ) {               // Construct full item name with prefix(es)
        itemname.append(strucPrefix[k]);
        if (k < nestlev) itemname.append(".");
      }
/* Create a new data item/value if: 
   1) the last colon was after the last '{' and 
   2) this was not a '}' or ',' immediately following another '}' or ','
*/
      string lstring = instring.substr(last_cppos, cppos-last_cppos+1);
      if (colpos >  oppos && !repeatTerm(lstring)) {
        addDataItem(strucName, value, itemname, valstring);
      }
 /* Also create a new data item/value in the special case
    where the data value is a flag list = "{ }" enclosed list */
      else if ((oppos > colpos) && (cppos > colpos) && repeatTerm(lstring)) {
        addDataItem(strucName, value, itemname, valstring);
      }

      if (instring[cppos] == '}') nestlev--;   // Decrement nest level at '}'
      last_cppos = cppos;                      // Remember closing parenthesis position
    } 
    
    else if (instring[j] == ':') {
    colpos = j;                                 // Colon position
    int spos = cppos > oppos ? cppos : oppos;   // Element name starts after last '{' or ',' 
    string partname = instring.substr(spos+1, colpos-spos-1);
    strucPrefix[nestlev] = partname;            // Save element name in nest level position
    }

  }
  
}

  std::string fileToString(const string fname)
// Read in the contents of a file into a single string
{
  ifstream inFile;
  string fstring="";

  char filename[1024];
  strcpy(filename, fname.c_str());
  inFile.open(filename, ios::in);
  if (!inFile)
    { cerr << "Could not open file " << fname << '\n'; return fstring; }
  stringstream strStream;
  strStream << inFile.rdbuf(); //read the file
  if (!inFile)
    { cerr << "Error reading file " << fname << '\n'; inFile.close(); return fstring; }
  fstring = strStream.str();    //fstring holds the content of the file
  inFile.close();
  return fstring;
}

