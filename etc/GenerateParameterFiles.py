import sys

if __name__ == "__main__":

    log_file_loc = "../documentation/logs/"
    out_file_loc = "../phantomCameraApp/src/"
    filenames = ["c1Struc.log","defcStruc.log","hwStruc.log","autoStruc.log", "cfStruc.log","camStruc.log","infoStruc.log"]

    of1 = open(out_file_loc + "phantomParamCreateCalls.h", 'w')
    of2 = open(out_file_loc + "phantomParamDefines.h", 'w')
    of3 = open(out_file_loc + "phantomParamVarDecl.h", 'w')
    for filename in filenames:
        filename = log_file_loc + filename
        f = open(filename, 'r+')
        names = []
        for line in f:
            if len(line.strip()) > 0:
              if ': {' in line and '}' not in line:
                  name = line.replace(": {", "").replace("\t", "").replace("\\", "").strip().capitalize()
                  names = names + [name]
              elif '}' in line and ': {' not in line:
                  names = names[:-1]
              else:
                  items = line.split(':')
                  paramName = "PHANTOM" + "".join(names) + items[0].replace("\t", "").replace("\\", "").strip().capitalize()
                  define = "PHANTOM_"
                  for name in names:
                    define = define + name.upper() + "_"
                  define = define + items[0].replace("\t", "").replace("\\", "").strip().upper()
                  # Check the type of the value
                  value = items[1].strip()
                  if '"' in value:
                    asyn_type = "asynParamOctet"
                  elif '{' in value and '}' in value:
                    asyn_type = "asynParamInt32"
                  else:
                    try:
                      int(value.replace(",", "").replace("\\", "").strip())
                      asyn_type = "asynParamInt32"
                    except ValueError:
                      try:
                        float(value.replace(",", "").replace("\\", "").strip())
                        asyn_type = "asynParamFloat64"
                      except ValueError:
                        try:
                          int(value.replace(",", "").replace("\\", "").strip(), 16)
                          asyn_type = "asynParamInt32"
                        except ValueError:
                          asyn_type = "asynParamOctet"
                  # Write out the entries to the output files
                  of1.write("createParam(" + paramName + "String, " + asyn_type + ", &" + paramName + "_);\n")
                  of2.write("#define " + paramName + "String \"" + define + "\"\n")
                  of3.write("int " + paramName + "_;\n")
    of1.close()
    of2.close()
    of3.close()

