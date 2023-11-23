#include <iostream>
#include <vector>
#include <map>
#include <PhantomCamera.h>
using namespace std;


   phantomCameraInfo info;
   phantomCameraHw hw;
   phantomCameraMeta meta;
   phantomCameraCam cam;
   phantomCameraAuto cameraAuto;
   phantomCameraEth eth;
   phantomCameraVideo video;
   phantomCameraIrig irig;
   phantomCameraMag mag;
   phantomCameraCf cf;
   phantomCameraUsets usets;
   phantomCameraDefc defc;
   phantomCameraCine cine;


int getIntParam(std::map<string, phantomVal> phantomParams, const string name)
{
  phantomVal value;
  value = phantomParams[name];
  if (value.getType()==PHANTOMTypeInteger) 
    return atoi((value.getValue()).c_str());
  else 
    return 0;
}

int main()
{
  string fileName[]={"c1Struc.log","defcStruc.log","hwStruc.log","autoStruc.log", 
                      "cfStruc.log","camStruc.log","infoStruc.log" };
  int numFile;
  unsigned int j;
  phantomVal phantomDat;
 
  std::string cameraDataIn;
  vector<string> names;
  vector<phantomVal> values;
  std::map<string, phantomVal> phantomParams;

  char* pPath;
  std::string sfilePath;
  pPath = getenv ("SFILEPATH");
  if (pPath!=NULL) {
    printf ("The current file path is: %s",pPath);
    sfilePath = pPath;
  }
  else {
    printf("Error - environment variable SFILEPATH is undefined\n");
    exit(1);
  }

  for (numFile = 0; numFile < 7 ; numFile++)
  {
    std::string fullFileName = sfilePath + '/' + fileName[numFile];
    cameraDataIn = fileToString(fullFileName);     // Read data from file
    string iline = stripControl(cameraDataIn);          // Delete unwanted chars from the received data

    j = 0;
    parseDataStruc(iline, names, values);
    cout << "Total " << names.size() << " data items. File read = " <<  fileName[numFile] << '\n';
    for (j=0; j < names.size(); j++ ) {
      phantomParams[names[j]] = values[j];  // Store in a Map structure
    }
  }

   FILE *fp1 = fopen("phantomParamCreateCalls.h","w");
   FILE *fp2 = fopen("phantomParamDefines.h","w");
   FILE *fp3 = fopen("phantomParamVarDecl.h","w");
   std::string parVarName ="";
   std::string parDefString ="";
// Iterate through the map structure, printing out all the [name,value+type]   
    std::map<string, phantomVal>::iterator mt;
    for(mt = phantomParams.begin(); mt != phantomParams.end(); ++mt) {
      cout << mt->first << "," << (mt->second).getValue()  << "," << (mt->second).getType() << "\n";
      std::string paramName = mt->first;
      parVarName = "";
      parDefString = "";
      for (size_t i=0; i<paramName.length(); ++i)
      {
        if (paramName.at(i) != '.') {
          if (i>0 && paramName.at(i-1) != '.')
            parVarName.push_back(paramName.at(i));
          else
            parVarName.push_back(toupper(paramName.at(i)));
        }
        if (paramName.at(i) == '.')
          parDefString.push_back('_');
        else
          parDefString.push_back(toupper(paramName.at(i)));
      }
      PHANTOMValueType_t mtype = (mt->second).getType();
      std::string sAsynType = "asynParamInt32";
      if (mtype == PHANTOMTypeFloat) {
        sAsynType = "asynParamFloat64";
      }
      else if (mtype == PHANTOMTypeString) {
        sAsynType = "asynParamOctet";
      }
      fprintf(fp1, "createParam(PHANTOM%s, %s, &PHANTOM%s_);\n", parVarName.c_str(), sAsynType.c_str(), 
                                                           parVarName.c_str());
      fprintf(fp2, "#define    PHANTOM%s  \"PHANTOM_%s\"\n", parVarName.c_str(),parDefString.c_str());
      fprintf(fp3, "int PHANTOM%s_;\n", parVarName.c_str());
    }
    fclose(fp1); fclose(fp2); fclose(fp3);
 
    info.model = phantomParams["info.model"].getValue();
    cout << "info.model = " << info.model << '\n';
    cout << "info.cinemem = " << getIntParam(phantomParams, "info.cinemem") << '\n';
    info.vbatt = atof((phantomParams["info.vbatt"].getValue()).c_str());
    cout << "info.vbatt = " << info.vbatt << '\n';
  return 0;
}

