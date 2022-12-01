//==============================================================================
//File name:    "analysis-graph.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page4.h"

//---- Mem Leakage Debug
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//----
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int CPage4::AnalysisGraph(HANDLE pArray)
{ 
  CArray <struct struct_analys, struct struct_analys> *pArr;
  pArr = (CArray <struct struct_analys, struct struct_analys>*)pArray;
 
  //===============================================
  //Create picture, Show graph
  //===============================================
  //AGRPH.Analysis = Analysis;
  AGRPH.SetHWnd(GetSafeHwnd());

  //By Y x10 = 0.1degr step 
  //AGRPH.SetMulY(10);
  //By X x10, 1Sec = 100px 
  //AGRPH.SetMulX(10); 
  //Add data

  //===========================================================
  //Предварительная обработка массива семплов
  //===========================================================
  //сколько всего групп точек из 6 каналов
  int total_samples = pArr->GetCount();
  
  //удалить сдвиг и нормализовать к началу = 0 точки по X
  unsigned short x0 = pArr->ElementAt(0).x;  
  for(int i=0; i<total_samples; i++)
  {
	unsigned short x = pArr->ElementAt(i).x - x0;
	pArr->ElementAt(i).x = x;
  }

  //пропорционально уменьшить шаг
  //int index_min = pArr->ElementAt(0).x;
  //получить максимальный индекс
  int index_max = pArr->ElementAt(total_samples-1).x; 
  //===========================================================

  //AGRPH.Parser(&x, &y, 0);//GRAPH_LOG
  //Загрузить данные в массивы каждой линии
  AGRPH.Parser(pArr);
  pArr->RemoveAll();

  //Set horisontal scale X, from 0 to AUTO, step 0.1 sec 
  //шаг данных картинки по горизонтали, число сегментов сетки
///  AGRPH.SetRangeX(0, pDevice->LastSample_number-x0, 8); //0,40,8

AGRPH.SetRangeX(0, 1, 8); //0,40,8
  
  //set vertical scale Y, from 40deg.. to 100, 0.1degr step 
  ///шаг данных по вертикали, число сегментов сетки  
  AGRPH.SetRangeY(0, 1024, 12);
 
  //Normalisation
  //сделать это если значения выходят за пределы шкалы
  //AGRPH.Normalization();
  
  //проверка работы алгоритма Bezier
  //AGRPH.BZI.TestBezier(100);

  //Interpolation R
  ///AGRPH.BZI.BezierResizer(&AGRPH.ArrR, 0, 40, 0, 1024);
  //AGRPH.BZI.BezierInterpolation(&AGRPH.ArrR);
  //Interpolation G
  //AGRPH.BZI.BezierResizer(&AGRPH.ArrG, 0, 40, 0, 1024);
  //Interpolation B
  ///AGRPH.BZI.BezierResizer(&AGRPH.ArrB, 0, 40, 0, 1024);
  //Interpolation Y
  ///AGRPH.BZI.BezierResizer(&AGRPH.ArrY, 0, 40, 0, 1024);

  //Height = 800 px = (100-20)*10, 0.1degr step	 "/Analysis-files/analysis"
  AGRPH.CreateGraph(640, 480, "analysis");

  //Show picture
  AGRPH.ShowGraph(NULL);

  return 1;
}
