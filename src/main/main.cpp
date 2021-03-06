#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include "../parser/lefdef/DefReader.h"
#include "../parser/lefdef/LefReader.h"
#include "../parser/lefdef/GuideReader.h"
#include "../impl/CreateLayer.h"


int main(){

  string def_file = "../benchmark/def/ispd18_test5.input.def";
  string lef_file = "../benchmark/lef/ispd18_test5.input.lef";
  string guide_file = "../benchmark/guide/ispd18_test5.input.guide";


  RawDataBase* db = new RawDataBase();
  MacroDataBase* macro_db = new MacroDataBase();
  GuideDataBase* gdb = new GuideDataBase();

  readDef(def_file, *db);
  readLef(lef_file, *macro_db);
  readGuide(guide_file, *gdb);

//  CreateLayer

  //Route(*db,*macro_db, *gdb);

  //printf("row name: %d\n",db->getRowArray()[0].step[0]); //test
  //printf("macro name: %f\n",macro_db->siteSizeX); //test

  //test
  /*
  int guidesize = gdb->getOriginalGuide().size();
  for (int i = 0; i < guidesize; ++i)
  {
    cout << gdb->getOriginalGuide()[i].netName << endl;
    cout << gdb->getOriginalGuide()[i].layerName << endl;
    cout << "x1 = " << gdb->getOriginalGuide()[i].x1 << " y1 = " << gdb->getOriginalGuide()[i].y1 << endl;
    cout << "x2 = " << gdb->getOriginalGuide()[i].x2 << " y2 = " << gdb->getOriginalGuide()[i].y2 << endl;
    cout << endl;
  }*/

  vector <Layer> layerArray;
  CreateLayer (*gdb, layerArray);


  //layer1 panel1 testbench
  int layersize = layerArray.size();
  cout << "Layer number: " << layersize << endl;
  int guidesize = layerArray[0].panelArray[0].guideArray.size();
  cout << "Layer1 Panel1 guide number is :" << guidesize << endl;
  for (int i = 0; i<guidesize; i++)
  {
   cout << "Guide bottom location:" << layerArray[0].panelArray[0].guideArray[i].bottom << endl; 
   cout << "Guide top location:" << layerArray[0].panelArray[0].guideArray[i].top << endl; 
   int netsize = layerArray[0].panelArray[0].guideArray[i].net.size();
   cout << " Has net:";
   for (int j = 0; j<netsize; j++)
   {
    cout << layerArray[0].panelArray[0].guideArray[i].net[j] << " ";
   }
   cout << endl;
  }

  return 0;
}
