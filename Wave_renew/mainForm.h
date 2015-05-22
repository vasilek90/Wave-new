#pragma once

#include<cmath>
#include<cstdio>
#include<iostream>
#include<fstream>

#include"defines.h"
#include"ViewForm.h"
#include <msclr/marshal.h>
#include"XMLConfigReader.h"

using namespace std;
using namespace System::Drawing;
using namespace msclr::interop;
using namespace Threading;

public delegate void processCalculationTimeDelegate(String^ s);
public delegate void generalDefaultsDelegate();
public delegate void pauseButtonDelegate(bool toggled);
public delegate int calculationDelegate();
public delegate void updateDrawDelegate();
public delegate void showRealTimeDelegate(int, int, int);

namespace Wave_renew
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	
	public ref class mainForm : public System::Windows::Forms::Form
	{

	private:
		System::Windows::Forms::Label^  label1;
		System::Windows::Forms::Label^  label2;
		System::Windows::Forms::Label^  label3;
		System::Windows::Forms::Label^  label4;
		System::Windows::Forms::Label^  label5;
		System::Windows::Forms::Label^  label6;
		System::Windows::Forms::Label^  label7;
		System::Windows::Forms::Label^  label8;
		System::Windows::Forms::Label^  label9;
		System::Windows::Forms::Label^  label10;
		System::Windows::Forms::Label^  label11;

		System::Windows::Forms::TextBox^  textBox_rangeX_start;
		System::Windows::Forms::TextBox^  textBox_rangeY_start;
		System::Windows::Forms::TextBox^  textBox_rangeX_end;
		System::Windows::Forms::TextBox^  textBox_rangeY_end;
		System::Windows::Forms::TextBox^  textBox_sizeX;
		System::Windows::Forms::TextBox^  textBox_sizeY;
		System::Windows::Forms::TextBox^  textBox_dx;
		System::Windows::Forms::TextBox^  textBox_dy;
		System::Windows::Forms::TextBox^  textBox_outTime;
		System::Windows::Forms::TextBox^  textBox_calcTime;
		System::Windows::Forms::TextBox^  textBox_isobath;

		System::Windows::Forms::Button^  button_applyParameters;
		System::Windows::Forms::Button^  button_startCalc;
		System::Windows::Forms::Button^  button_pauseCalc;
		System::Windows::Forms::Button^  button_stopCalc;

		System::Windows::Forms::ToolStripMenuItem^  ToolStrip_file;
		System::Windows::Forms::ToolStripMenuItem^  ToolStrip_file_openMap;
		System::Windows::Forms::ToolStripMenuItem^  ToolStrip_file_openConfig;

		System::ComponentModel::Container ^components;
		System::Windows::Forms::MenuStrip^  menuStrip_main;
		System::Windows::Forms::CheckBox^  checkBox_autoSaveLayers;

		ViewForm^ vf;
		Graphics^ mainGraphics;
		Bitmap^ mainBitmap;
		String^ mapFileName;
		Thread^ calculationThread;

			 Thread^ drawingThread;

	public:
		mainForm()
		{
			InitializeComponent();
		}

	protected:
		~mainForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		double** allocateMemory(int _y, int _x)
		{
			double **_m = new double *[_y + 1];
			if (!_m) 
				return 0;
			for (int j = 0; j < _y; j++)
				if (!(_m[j] = new double[_x])) 
				{
					for (; j >= 0; j--) 
						delete _m[j];
					return 0;
				}
			_m[_y] = NULL;
			return _m;
		}

		void deallocateMemory(double **_m)
		{
			for (int j = 0; _m[j] != NULL; j++)
				delete _m[j];
			delete _m;
		}

		void swapMemory(double ***_m1, double ***_m2)
		{
			double **tmp = *_m1;
			*_m1 = *_m2;
			*_m2 = tmp;
		}

		Color bottom2color(double h)
		{
			const int h_max = 80; // = sqrt(5000)
			const int h_min = -110;// = -sqrt(10000)
			if (h>0) 
				h = sqrt(h); 
			else 
				if (h < 0) 
					h = -sqrt(-h);
			if (h >= 0) 
				return Color::FromArgb(0, 55 + h * 200 / h_max, 0);
			else 
				return Color::FromArgb(0, 0, 255 - h * 255 / h_min);
		}

		Color eta2color(double h)
		{
			const int h_max = 72; // = sqrt(5000)
			const int h_minmax = 10; // = sqrt(25)
			const int h_maxmax = 15; // = sqrt(25)
			if (h >= LAND_UP)
			{
				h = sqrt(h - LAND_UP);
				//return Color::FromArgb(0, 55 + h * 200 / h_max, 0);
				return Color::FromArgb(0, 255, 0);
			}
			else 
				if (h <= h_maxmax)
				{
					if (h > h_minmax + 2)
						return Color::FromArgb(161, 19, 33);
					if (h > h_minmax)
						return Color::FromArgb(200, 0, 200);
					if (h > h_minmax - 2)
						return Color::FromArgb(120, 100, 200);
					if (h > h_minmax - 3)
						return Color::FromArgb(25, 25, 225);
					if (h > h_minmax - 4)
						return Color::FromArgb(150, 150, 250);
					if (h > h_minmax - 5)
						return Color::FromArgb(100, 220, 220);
					if (h > h_minmax - 6)
						return Color::FromArgb(200, 220, 220);
					if (h > h_minmax - 6.5)
						return Color::FromArgb(100, 255, 100);
					if (h > h_minmax - 7)
						return Color::FromArgb(133, 27, 181);
					if (h > h_minmax - 7.5)
						return Color::FromArgb(250, 250, 0);
					if (h > h_minmax - 8)
						return Color::FromArgb(255, 181, 41);
					if (h > h_minmax - 8.5)
						return Color::FromArgb(255, 200, 200);
					if (h > h_minmax - 9)
						return Color::FromArgb(255, 0, 0);
					if (h > h_minmax - 9.5)
						return Color::FromArgb(255, 100, 0);
					if (h > h_minmax - 9.99)
						return Color::FromArgb(166, 249, 45);
					else 
						if (h > -h_maxmax)
							return Color::FromArgb(0, 0, 70 + h * 70 / h_maxmax);
						else
							return Color::FromArgb(125, 25, 225);
				}
				else
				{
					h = sqrt(h);
					return Color::FromArgb(0, h * 255 / h_max / 2, 255 - h * 255 / h_max / 2);	
				}
		}

		Color height2color(double h)
		{
			const int h_max = 71; // = sqrt(5000)
			const int h_minmax = 10;// = sqrt(400)
			if (h > LAND_UP)
			{
				h = sqrt(h - LAND_UP);
				return Color::FromArgb(0, 55 + h * 200 / h_max, 0);
			}
			else
			{
				if (h > 0) h = sqrt(h); else if (h < 0) h = -sqrt(-h);
				return Color::FromArgb(0, 0, 127 + h * 127 / h_minmax);
			}
		}

		void showBottom()
		{

			for (int y = 0; y < mapSizeY; y++)
			{
				for (int x = 0; x < mapSizeX; x++)
				{
					try
					{
						this->mainBitmap->SetPixel(x, mapSizeY - 1 - y, bottom2color(terrian[y][x]));
					}
					catch(...)
					{
						double t4 = terrian[y][x-3];
						double t3 = terrian[y][x-2];
						double t2 = terrian[y][x-1];
						double t1 = terrian[y][x];
					}
				}
			}


			this->Invoke_updateDraw();
		}

		void showDisturbance()
		{
			if (!waveFrontCurrent)
				return;

			//double **a = allocateMemory(mapSizeY, mapSizeX);
			for (int y = 0; y < mapSizeY; y++)
				for (int x = 0; x < mapSizeX; x++)
					if (terrian[y][x] < 0)
					{
						double val = waveFrontCurrent[y][x];
						this->mainBitmap->SetPixel(x, mapSizeY - 1 - y, eta2color(val));
					}
					else
					{
						double val = terrian[y][x] + LAND_UP;
						this->mainBitmap->SetPixel(x, mapSizeY - 1 - y, eta2color(val));
					}

			this->Invoke_updateDraw();
		}

		void ShowHeights()
		{
			double **a = allocateMemory(mapSizeY, mapSizeX);
			for (int y = 0; y < mapSizeY; y++)
			{
				for (int x = 0; x < mapSizeX; x++)
				{
					if (terrian[y][x] < 0)
					{
						double val = heightsFront[y][x];
						this->mainBitmap->SetPixel(x, mapSizeY - 1 - y, height2color(val));
					}
					else
					{
						double val = terrian[y][x] + LAND_UP;
						this->mainBitmap->SetPixel(x, mapSizeY - 1 - y, height2color(val));
					}
				}
			}

			this->Invoke_updateDraw();
		}

		void OutHeights(string fileName)
		{
			ofstream outFile;
			outFile.open(fileName.c_str(), ios::out);

			for (int i = 0; i < mapSizeY; i++)
			{
				double h(0.0);
				for (int j = 0; j < mapSizeX - 1; j++)
				{
					if (terrian[i][j] < 0)
						h = heightsFront[i][j];
					else
						h = terrian[i][j] + LAND_UP;

					outFile << h << "; ";
				}

				if (terrian[i][mapSizeX - 1] < 0)
					h = 0.0;
				else
					h = terrian[i][mapSizeX - 1] + LAND_UP;

				outFile << h << endl;
			}

			outFile.close();
		}

		void OutHeights(int time)
		{
			char tmpFileName[20];
			sprintf_s(tmpFileName, "out_t=%06d.grd", time);

			ofstream outFile;
			outFile.open(tmpFileName, ios::out);

			double maxPoint(-999999), minPoint(999999);
			for (int i = 0; i < mapSizeY; i++)
			{
				double h(0.0);
				for (int j = 0; j < mapSizeX; j++)
				{
					if (terrian[i][j] < 0)
						h = waveFrontCurrent[i][j];
					else
						h = terrian[i][j] + LAND_UP;
					if (h > maxPoint)
						maxPoint = h;
					if (h < minPoint)
						minPoint = h;
				}
			}

			outFile << "DSAA" << endl;
			outFile << mapSizeX << " " << mapSizeY << endl;
			outFile << startX_dgr << " " << endX_dgr << endl;
			outFile << startY_dgr << " " << endY_dgr << endl;
			outFile << minPoint << " " << maxPoint << endl;
			for (int i = 0; i < mapSizeY; i++)
			{
				double h(0.0);
				for (int j = 0; j < mapSizeX - 1; j++)
				{
					if (terrian[i][j] < 0)
						h = waveFrontCurrent[i][j];
					else
						h = 0;

					outFile << h << " ";
				}

				if (terrian[i][mapSizeX - 1] < 0)
					h = waveFrontCurrent[i][mapSizeX - 1];
				else
					h = 0;

				outFile << h << endl;
			}
			outFile.close();
		}

		bool loadMapDat()
		{
			ifstream mapFile;
			
			marshal_context^ context = gcnew marshal_context();
			const char* tmpFileName = context->marshal_as<const char*>(mapFileName);
				
			mapFile.open(tmpFileName, ios::in);
			if (!mapFile)
			{
				MessageBox::Show("Input file not found!", "Error!", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return false;
			}
			delete context;

			long double minx;
			long double maxx;
			long double miny;
			long double maxy;
			long double longitude;
			long double latitude;
			long double depth;
			bool begin=true;
			bool check_deltax=true;
			bool check_deltay=true;
			long double deltax;
			long double deltay;

			//FILE* infile;
			//infile = fopen(tmpFileName, "rt");
			//while(feof(infile)==0)
			while(mapFile.peek()!=EOF)
			{
				//fscanf(infile,"%f %f %f", &latitude, &longitude, &depth);
				mapFile >>latitude >> longitude>>depth;
				if(begin)
				{
					maxx=minx=latitude;
					maxy=miny=longitude;
					begin=false;
				} else {
					if(check_deltax)
					{
						deltax=latitude-minx;
						if(deltax!=0)
						{
							if(deltax<0)deltax=-deltax;
							check_deltax=false;
						}
					};

				if(check_deltay)
				{
					deltay=longitude-miny;
					if(deltay!=0)
					{
						if(deltay<0)deltay=-deltay;
						check_deltay=false;
					}
				};

        if(maxx < latitude)maxx=latitude;
        else if(minx > latitude)minx=latitude;

        if(maxy < longitude)maxy=longitude;
        else if(miny > longitude)miny=longitude;
      };
    };

			mapFile.close();
			ifstream mapFile2;
			context = gcnew marshal_context();
			tmpFileName = context->marshal_as<const char*>(mapFileName);

			mapFile2.open(tmpFileName, ios::in);
			//mapFile2.seekg(ios::beg);
			if (!mapFile2)
			{
				MessageBox::Show("Input file not found!", "Error!", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return false;
			}
	int sx = (int)((maxx-minx)/deltax)+1;
    int sy = (int)((maxy-miny)/deltay)+1;
			
			mapSizeX = sx;//nnnnn
			mapSizeY = sy;
			startX_dgr = minx;
			endX_dgr = maxx;
			startY_dgr = miny;
			endY_dgr = maxy;

			if (terrian)
				deallocateMemory(terrian);
			terrian = allocateMemory(mapSizeY, mapSizeX);

			long double i=0, j=0;
			ofstream mapFile3;
			mapFile3.open("out.txt", ios::out);
			int correcti = 0, correctj =0, previ = 0, prevj =0;
			while(mapFile2.peek()!=EOF)
			{
				mapFile2 >>latitude>>longitude>> depth;
				long double tmpi, tmpj;
				tmpi = ((long double)sx / (long double)(maxx- minx) * (long double)(latitude - minx));
				tmpj = ((long double)sy / (long double)(maxy- miny) * (long double)(longitude - miny));
				i =  truncf(tmpi) - correcti;
				j = truncf(tmpj) - correctj;
				if ((((int)i - previ)>1) && (i != 0))
				{
					correcti++;
					i--;
				}
				if ((((int)j - prevj)>1) && (j != 0))
				{
					correctj++;
					j--;
				}

				if (i<0)
				{
					i=0;
				}
				if (j<0)
				{
					j=0;
				}

				previ = i;
				prevj = j;
				
				mapFile3 << i <<" "<<j<<" "<<depth<<" "<<correcti<<" "<<correctj<<endl;
				if ((int)i != sx && (int)j != sy)
					terrian[(int)j][(int)i] = depth;
			}
			mapFile3.close();
			this->textBox_rangeX_start->Text = System::Convert::ToString(startX_dgr);
			this->textBox_rangeX_end->Text = System::Convert::ToString(endX_dgr);
			this->textBox_rangeY_start->Text = System::Convert::ToString(startY_dgr);
			this->textBox_rangeY_end->Text = System::Convert::ToString(endY_dgr);
			this->textBox_sizeX->Text = System::Convert::ToString(mapSizeX);
			this->textBox_sizeY->Text = System::Convert::ToString(mapSizeY);
			delta_x = (endX_dgr - startX_dgr) / (mapSizeX - 1);
			delta_y = (endY_dgr - startY_dgr) / (mapSizeY - 1);
			this->textBox_dx->Text = System::Convert::ToString(delta_x);
			this->textBox_dy->Text = System::Convert::ToString(delta_y);

			mapFile2.close();

			return true;
		}

		float truncf(float x)
		{       
			return x < 0.0f ? ceilf(x-0.5) : floorf(x+0.5);
		}

		bool loadMapMtx()
		{
			ifstream mapFile;
			
			marshal_context^ context = gcnew marshal_context();
			const char* tmpFileName = context->marshal_as<const char*>(mapFileName);
				
			mapFile.open(tmpFileName, ios::in);
			if (!mapFile)
			{
				MessageBox::Show("Input file not found!", "Error!", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return false;
			}
			delete context;

			const int param_cnt = 8;
			int param[param_cnt];
			array<String^>^ sparam = gcnew array<String^>(param_cnt) { "version", "size_x", "size_y", "start_x", "end_x", "start_y", "end_y", "data:"};

			char cs[MAX_STR_LEN];

			for (int p = 0; p < param_cnt; p++)
			{
				mapFile.getline(cs, MAX_STR_LEN);

				String^ s = gcnew String(cs);
				if (s->ToLower()->IndexOf(sparam[p]) == -1)
				{
					MessageBox::Show("Wrong file format!", "Error!", MessageBoxButtons::OK, MessageBoxIcon::Error);
					return false;
				}
				int pos = s->IndexOf("=");
				if (p != 7)
					param[p] = (int)System::Convert::ToDouble(s->Substring(pos + 1, s->Length - pos - 1)->Trim());
			}

			mapSizeX = param[1];
			mapSizeY = param[2];
			startX_dgr = param[3];
			endX_dgr = param[4];
			startY_dgr = param[5];
			endY_dgr = param[6];

			if (terrian)
				deallocateMemory(terrian);
			terrian = allocateMemory(mapSizeY, mapSizeX);

			for (int y = 0; y < mapSizeY; y++)
				for (int x = 0; x < mapSizeX; x++)
					mapFile >> terrian[y][x];

			this->textBox_rangeX_start->Text = System::Convert::ToString(startX_dgr);
			this->textBox_rangeX_end->Text = System::Convert::ToString(endX_dgr);
			this->textBox_rangeY_start->Text = System::Convert::ToString(startY_dgr);
			this->textBox_rangeY_end->Text = System::Convert::ToString(endY_dgr);
			this->textBox_sizeX->Text = System::Convert::ToString(mapSizeX);
			this->textBox_sizeY->Text = System::Convert::ToString(mapSizeY);
			delta_x = (endX_dgr - startX_dgr) / (mapSizeX - 1);
			delta_y = (endY_dgr - startY_dgr) / (mapSizeY - 1);
			this->textBox_dx->Text = System::Convert::ToString(delta_x);
			this->textBox_dy->Text = System::Convert::ToString(delta_y);

			mapFile.close();

			return true;
		}

		void SaveBmp(int time)
		{
			mainBitmap->Save("output_t" + System::Convert::ToString(time) + ".bmp");
		}

		void fill_tetragon(int** terr, int v,
			int mini, int minj, int maxi, int maxj,
			int i1, int j1,
			int i2, int j2,
			int i3, int j3,
			int i4, int j4)
		{
			int moving = abs(i1 - i2) + abs(i2 - i3) + abs(i3 - i4) + abs(i4 - i1) +
				abs(j1 - j2) + abs(j2 - j3) + abs(j3 - j4) + abs(j4 - j1);
			double x, y;
			int promi, promj;

			for (x = 0; x <= 1; x = x + 1.0 / (double)(moving))
			{
				for (y = 0; y <= 1; y = y + 1.0 / (double)(moving))
				{
					promi = int(x*(i1*y + i2*(1 - y)) + (1 - x)*(i3*y + i4*(1 - y)) - 0.5);
					promj = int(x*(j1*y + j2*(1 - y)) + (1 - x)*(j3*y + j4*(1 - y)) - 0.5);
					if (promi >= 0 && promj >= 0 && promi <= mapSizeX - 2 && promj <= mapSizeY - 2)terr[promj][promi] = v;
				}

			}
		}

		void tmp()
		{
			hearthBricksQ = 2;

			terr_tmp = new double*[hearthBricksQ];
			for (int i = 0; i < hearthBricksQ; i++)
				terr_tmp[i] = new double[10];

			terr_tmp[0][0] = 30;
			terr_tmp[0][1] = 5;
			terr_tmp[0][2] = 288.64;
			terr_tmp[0][3] = -19.04;
			terr_tmp[0][4] = 289.36;
			terr_tmp[0][5] = -20.48;
			terr_tmp[0][6] = 288.8;
			terr_tmp[0][7] = -19;
			terr_tmp[0][8] = 289.52;
			terr_tmp[0][9] = -20.4;

			terr_tmp[1][0] = 10;
			terr_tmp[1][1] = -1;
			terr_tmp[1][2] = 288.8;
			terr_tmp[1][3] = -19;
			terr_tmp[1][4] = 289.52;
			terr_tmp[1][5] = -20.4;
			terr_tmp[1][6] = 288.96;
			terr_tmp[1][7] = -18.96;
			terr_tmp[1][8] = 289.68;
			terr_tmp[1][9] = -20.32;
		}

		void tmp2()
		{
			hearthBricksQ = 3;

			terr_tmp = new double*[hearthBricksQ];
			for (int i = 0; i < hearthBricksQ; i++)
				terr_tmp[i] = new double[10];

			terr_tmp[0][0] = 10;
			terr_tmp[0][1] = 1;
			terr_tmp[0][2] = 288.63;
			terr_tmp[0][3] = -19.42;
			terr_tmp[0][4] = 288.8;
			terr_tmp[0][5] = -19.73;
			terr_tmp[0][6] = 289.38;
			terr_tmp[0][7] = -19.17;
			terr_tmp[0][8] = 289.53;
			terr_tmp[0][9] = -19.39;

			terr_tmp[1][0] = 30;
			terr_tmp[1][1] = 6;
			terr_tmp[1][2] = 288.8;
			terr_tmp[1][3] = -19.73;
			terr_tmp[1][4] = 289.97;
			terr_tmp[1][5] = -20.02;
			terr_tmp[1][6] = 289.53;
			terr_tmp[1][7] = -19.39;
			terr_tmp[1][8] = 289.86;
			terr_tmp[1][9] = -19.89;

			terr_tmp[2][0] = 10;
			terr_tmp[2][1] = 1;
			terr_tmp[2][2] = 288.97;
			terr_tmp[2][3] = -20.2;
			terr_tmp[2][4] = 289.03;
			terr_tmp[2][5] = -20.86;
			terr_tmp[2][6] = 289.86;
			terr_tmp[2][7] = -19.89;
			terr_tmp[2][8] = 289.97;
			terr_tmp[2][9] = -20.72;
		}

		int mainForm::processing()
		{
			//tmp2();

			int old_x = 0;
			int old_y = 0;
			isProcessing = true;
			if (h) 
				deallocateMemory(h);
			if (waveFrontCurrent)
				deallocateMemory(waveFrontCurrent);
			if (waveFrontOld)
				deallocateMemory(waveFrontOld);
			if (heightsFront)
				deallocateMemory(heightsFront);
			if (uCurrent)
				deallocateMemory(uCurrent);
			if (vCurrent)
				deallocateMemory(vCurrent);
			if (uOld)
				deallocateMemory(uOld);
			if (vOld)
				deallocateMemory(vOld);

			h = allocateMemory(mapSizeY, mapSizeX);
			waveFrontCurrent = allocateMemory(mapSizeY, mapSizeX);
			waveFrontOld = allocateMemory(mapSizeY, mapSizeX);
			uCurrent = allocateMemory(mapSizeY, mapSizeX);
			vCurrent = allocateMemory(mapSizeY, mapSizeX);
			uOld = allocateMemory(mapSizeY, mapSizeX);
			vOld = allocateMemory(mapSizeY, mapSizeX);
			heightsFront = allocateMemory(mapSizeY, mapSizeX);
			if (!(terrian || h || waveFrontCurrent || waveFrontOld || heightsFront || uCurrent || vCurrent || uOld || vOld))
				return 2;

			if (delta_y_m)	
				delete delta_y_m;
			delta_y_m = new double[mapSizeY];

			if (delta_t) 
				delete delta_t;
			delta_t = new double[mapSizeY];

			if (terr_points) 
				delete terr_points;
			terr_points = allocateMemory(8, hearthBricksQ);

			if (point_points) 
				delete point_points;
			point_points = allocateMemory(2, watchingPointsQ);

			if (t_h_v_up) 
				delete t_h_v_up;
			t_h_v_up = allocateMemory(8, hearthBricksQ);

			if (terr_up)
			{
				for (int j = 0; terr_up[j] != NULL; j++) 
					delete terr_up[j];
				delete terr_up;
			}
			terr_up = new int *[mapSizeY + 1];
			for (int j = 0; j<mapSizeY; j++)
				terr_up[j] = new int[mapSizeX];
			terr_up[mapSizeY] = NULL;

			calculationTime = System::Convert::ToInt32(this->textBox_calcTime->Text);
			outTime = System::Convert::ToInt32(this->textBox_outTime->Text);

			//****************************************************************************
			for (int y = 0; y<mapSizeY; y++)
			{
				for (int x = 0; x < mapSizeX; x++)
				{
					if (terrian[y][x] <= 0 && terrian[y][x] >= isobath)
						h[y][x] = isobath;
					else 
						if (terrian[y][x] >= 0)
							h[y][x] = -1;//sqrt(-h[j][i]);
						else 
							h[y][x] = terrian[y][x];

					waveFrontOld[y][x] = 0;
					uOld[y][x] = 0;
					vOld[y][x] = 0;
					terr_up[y][x] = 0;

					if (maxHeight > terrian[y][x])
						maxHeight = terrian[y][x];
				}
			}
			//*****************************************************************************

			for (int j = 0; j<hearthBricksQ; j++)
			{
				t_h_v_up[0][j] = terr_tmp[j][0];
				t_h_v_up[1][j] = terr_tmp[j][1];
				t_h_v_up[2][j] = t_h_v_up[1][j] / t_h_v_up[0][j];

				for (int i = 0; i<8; i++)
					terr_points[i][j] = terr_tmp[j][i + 2];

				fill_tetragon(terr_up, j + 1,
					0, 0, mapSizeX - 1, mapSizeY - 1,
					(terr_points[0][j] - startX_dgr) / delta_x + 0.5, (terr_points[1][j] - startY_dgr) / delta_y + 0.5,
					(terr_points[2][j] - startX_dgr) / delta_x + 0.5, (terr_points[3][j] - startY_dgr) / delta_y + 0.5,
					(terr_points[4][j] - startX_dgr) / delta_x + 0.5, (terr_points[5][j] - startY_dgr) / delta_y + 0.5,
					(terr_points[6][j] - startX_dgr) / delta_x + 0.5, (terr_points[7][j] - startY_dgr) / delta_y + 0.5
					);
			}

			delta_x_m = delta_x * M_PI * EARTH_RADIUS_M / 180.0;

			//***************************************************************************
			for (int j = 0; j<mapSizeY; j++)
			{
				delta_y_m[j] = delta_x_m*cos((startY_dgr + j*delta_y) / 180.0*M_PI);
				delta_t[j] = delta_x_m*delta_y_m[j]/sqrt(-M_G*maxHeight*(delta_x_m*delta_x_m + delta_y_m[j]*delta_y_m[j]));
			}
			//   if (delta_t > delta_y_m/sqrt(2*M_G*9500))
			//      delta_t = delta_y_m/sqrt(2*M_G*9500);

			double EarthSpeed = EQUATOR_LENGTH_KM / EARTH_TURN_TIME_S;
			//float Koef_Sheroh=0.002;
			//float Koef_Sh=-M_G*Koef_Sheroh*Koef_Sheroh;
			for (currentCalculationTime = 0; currentCalculationTime <= calculationTime; currentCalculationTime++)
			{
				this->Invoke_button_startCalc_changeText(System::Convert::ToString(currentCalculationTime));

				for (int j = 1; j<mapSizeY - 1; j++)
				{
					double CoriolisEffectKoef = 2.0 * EarthSpeed * cos((startY_dgr + j*delta_y) / 180.0*M_PI);
					for (int i = 1; i<mapSizeX - 1; i++)
					{
						if (i<mapSizeX - 2 && j<mapSizeY - 2)
							waveFrontCurrent[j][i] = waveFrontOld[j][i] + delta_t[j] \
							* (0.5 / delta_x_m * (uOld[j + 1][i] * (h[j + 2][i] + h[j + 1][i]) \
							- uOld[j][i] * (h[j + 1][i] + h[j][i])) \
							+ 0.5 / delta_y_m[j] * (vOld[j][i + 1] * (h[j][i + 2] + h[j][i + 1]) \
							- vOld[j][i] * (h[j][i + 1] + h[j][i])));

						if (i > 0 && j > 0)
						{
							uCurrent[j][i] = uOld[j][i] - (M_G / (2 * delta_x_m)*(waveFrontCurrent[j][i] - waveFrontCurrent[j - 1][i])
								- CoriolisEffectKoef*vOld[j][i]
								//-Koef_Sh/exp(1.8*log(fabs(h[j][i]+eta[j][i])))
								//*u_old[j][i]*sqrt(u_old[j][i]*u_old[j][i]+v_old[j][i]*v_old[j][i])
								)*delta_t[j];
							vCurrent[j][i] = vOld[j][i] - (M_G / (2 * delta_y_m[j])*(waveFrontCurrent[j][i] - waveFrontCurrent[j][i - 1])
								+ CoriolisEffectKoef*uOld[j][i]
								//-Koef_Sh/exp(1.8*log(fabs(h[j][i]+eta[j][i])))
								//*v_old[j][i]*sqrt(u_old[j][i]*u_old[j][i]+v_old[j][i]*v_old[j][i])
								)*delta_t[j];
						}

						for (int b = 0; b < hearthBricksQ; b++)
							if (terr_up[j][i] == b + 1 && currentCalculationTime*delta_t[j] < t_h_v_up[0][b] - delta_t[j])
								waveFrontCurrent[j][i] = waveFrontCurrent[j][i] + t_h_v_up[2][b] * delta_t[j];
					}
				}

				for (int i = 1; i<mapSizeX; i++)
				{
					int temp = (int)(i*mapSizeY / mapSizeX);
					vCurrent[0][i] = sqrt(-M_G*h[0][i])*waveFrontCurrent[0][i] / (waveFrontCurrent[1][i] - h[0][i]);
					vCurrent[mapSizeY - 2][i] = sqrt(-M_G*h[mapSizeY - 2][i])*waveFrontCurrent[mapSizeY - 3][i] / (waveFrontCurrent[mapSizeY - 3][i] - h[mapSizeY - 2][i]);;//v[size_y-3][i];//
					vCurrent[mapSizeY - 1][i] = vCurrent[mapSizeY - 2][i];// sqrt(-M_G*h[size_y-1][i])*eta[size_y-1][i]/(eta[size_y-2][i]-h[size_y-1][i]);
					//v[size_y][i] =v[size_y-1][i];
					waveFrontCurrent[0][i] = waveFrontOld[0][i] - sqrt(-h[0][i] * M_G)*(delta_t[temp] / delta_y_m[temp])*(waveFrontOld[0][i] - waveFrontOld[1][i]);
					waveFrontCurrent[mapSizeY - 2][i] = waveFrontOld[mapSizeY - 2][i] - sqrt(-h[mapSizeY - 2][i] * M_G)*(delta_t[temp] / delta_y_m[temp])*(waveFrontOld[mapSizeY - 2][i] - waveFrontOld[mapSizeY - 3][i]);
					waveFrontCurrent[mapSizeY - 1][i] = waveFrontCurrent[mapSizeY - 2][i];// - sqrt(-h[size_y-1][i]*M_G)*(delta_t[i]/delta_y_m[i])*(eta_old[size_y-1][i]-eta_old[size_y-2][i]);
				}

				for (int j = 1; j<mapSizeY; j++)
				{
					uCurrent[j][0] = sqrt(-M_G*h[j][0])*waveFrontCurrent[j][1] / (waveFrontCurrent[j][1] - h[j][0]);
					uCurrent[j][mapSizeX - 2] = uCurrent[j][mapSizeX - 3];//-sqrt(-M_G*h[j][size_x-2])*eta[j][size_x-3]/(eta[j][size_x-3]-h[j][size_x-2])
					uCurrent[j][mapSizeX - 1] = uCurrent[j][mapSizeX - 2];// = sqrt(-M_G*h[j][size_x-1])*eta[j][size_x-1]/(eta[j][size_x-2]-h[j][size_x-1]);
					//u[j][size_x] = u[j][size_x-1];
					waveFrontCurrent[j][0] = waveFrontOld[j][0] - sqrt(-h[j][0] * M_G)*(delta_t[j] / delta_x_m)*(waveFrontOld[j][0] - waveFrontOld[j][1]);
					waveFrontCurrent[j][mapSizeX - 2] = waveFrontOld[j][mapSizeX - 2] - sqrt(-h[j][mapSizeX - 2] * M_G)*(delta_t[j] / delta_x_m)*(waveFrontOld[j][mapSizeX - 2] - waveFrontOld[j][mapSizeX - 3]);
					waveFrontCurrent[j][mapSizeX - 1] = waveFrontCurrent[j][mapSizeX - 2];// - sqrt(-h[j][size_x-1]*M_G)*(delta_t[j]/delta_x_m)*(eta_old[j][size_x-1]-eta_old[j][size_x-2]);
				}

				for (int j = 1; j<mapSizeY - 1; j++)
				{
					for (int i = 1; i<mapSizeX - 1; i++)
					{
						if (waveFrontCurrent[j][i] > 15.)
						{ 
							waveFrontCurrent[j][i] = 15.;
						}
						if (waveFrontCurrent[j][i] < -15.)
						{ 
							waveFrontCurrent[j][i] = -15.;
						}
						if (heightsFront[j][i] < waveFrontCurrent[j][i])
						{ 
							heightsFront[j][i] = waveFrontCurrent[j][i];
						}
					}
				}

				swapMemory(&waveFrontOld, &waveFrontCurrent);
				swapMemory(&vOld, &vCurrent);
				swapMemory(&uOld, &uCurrent);

				if (outTime <= currentCalculationTime && currentCalculationTime % outTime == 0)
				{
					int h = (int)(delta_t[0] * currentCalculationTime) / 3600;
					int m = (int)((delta_t[0] * currentCalculationTime) - h * 3600) / 60;
					int s = (int)(delta_t[0] * currentCalculationTime) - h * 3600 - m * 60;

					Invoke_showRealTime(h, m, s);
					showDisturbance();

					if (this->checkBox_autoSaveLayers->Checked)
						OutHeights(currentCalculationTime);

				}
				if (!isProcessing)
				{
					this->Invoke_afterCalcDefaults();
					break;
				}
			}

			MessageBox::Show("Modelling Complete!", "Information!", MessageBoxButtons::OK, MessageBoxIcon::Information);
			this->Invoke_afterCalcDefaults();

			return 0;
		}

		void checkReadyForCalculationState()
		{
			if (!isProcessing)
			{
				if (this->textBox_calcTime->Text != "" && this->textBox_isobath->Text != "" && this->textBox_outTime->Text != "")
				{
					if (System::Convert::ToDouble(this->textBox_calcTime->Text) > 0 && System::Convert::ToDouble(this->textBox_outTime->Text))
					{
						this->button_applyParameters->Enabled = true;
					}
					else
						this->button_applyParameters->Enabled = false;
				}
				else
				{
					this->button_applyParameters->Enabled = false;
				}
			}
			else
				if (System::Convert::ToDouble(this->textBox_calcTime->Text)>currentCalculationTime)
				{
					this->button_applyParameters->Enabled = true;
				}
				else
					this->button_applyParameters->Enabled = false;
		}

		int test()
		{
			MessageBox::Show("ok", "ok", MessageBoxButtons::OK, MessageBoxIcon::None);
			//showBottom();
			return 0;
		}

		void blabla()
		{
			processing();
		}

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->textBox_rangeX_start = (gcnew System::Windows::Forms::TextBox());
			this->textBox_rangeY_start = (gcnew System::Windows::Forms::TextBox());
			this->textBox_rangeX_end = (gcnew System::Windows::Forms::TextBox());
			this->textBox_rangeY_end = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->textBox_sizeX = (gcnew System::Windows::Forms::TextBox());
			this->textBox_sizeY = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->textBox_dx = (gcnew System::Windows::Forms::TextBox());
			this->textBox_dy = (gcnew System::Windows::Forms::TextBox());
			this->textBox_outTime = (gcnew System::Windows::Forms::TextBox());
			this->textBox_calcTime = (gcnew System::Windows::Forms::TextBox());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->button_applyParameters = (gcnew System::Windows::Forms::Button());
			this->button_startCalc = (gcnew System::Windows::Forms::Button());
			this->button_pauseCalc = (gcnew System::Windows::Forms::Button());
			this->button_stopCalc = (gcnew System::Windows::Forms::Button());
			this->menuStrip_main = (gcnew System::Windows::Forms::MenuStrip());
			this->ToolStrip_file = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStrip_file_openMap = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStrip_file_openConfig = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->textBox_isobath = (gcnew System::Windows::Forms::TextBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->checkBox_autoSaveLayers = (gcnew System::Windows::Forms::CheckBox());
			this->menuStrip_main->SuspendLayout();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 36);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(49, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Range X";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(98, 36);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(10, 13);
			this->label2->TabIndex = 1;
			this->label2->Text = L"-";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(12, 58);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(49, 13);
			this->label3->TabIndex = 2;
			this->label3->Text = L"Range Y";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(98, 58);
			this->label4->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(10, 13);
			this->label4->TabIndex = 3;
			this->label4->Text = L"-";
			// 
			// textBox_rangeX_start
			// 
			this->textBox_rangeX_start->Location = System::Drawing::Point(64, 33);
			this->textBox_rangeX_start->Margin = System::Windows::Forms::Padding(2);
			this->textBox_rangeX_start->Name = L"textBox_rangeX_start";
			this->textBox_rangeX_start->ReadOnly = true;
			this->textBox_rangeX_start->Size = System::Drawing::Size(30, 20);
			this->textBox_rangeX_start->TabIndex = 4;
			this->textBox_rangeX_start->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// textBox_rangeY_start
			// 
			this->textBox_rangeY_start->Location = System::Drawing::Point(64, 55);
			this->textBox_rangeY_start->Margin = System::Windows::Forms::Padding(2);
			this->textBox_rangeY_start->Name = L"textBox_rangeY_start";
			this->textBox_rangeY_start->ReadOnly = true;
			this->textBox_rangeY_start->Size = System::Drawing::Size(30, 20);
			this->textBox_rangeY_start->TabIndex = 5;
			this->textBox_rangeY_start->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// textBox_rangeX_end
			// 
			this->textBox_rangeX_end->Location = System::Drawing::Point(112, 33);
			this->textBox_rangeX_end->Margin = System::Windows::Forms::Padding(2);
			this->textBox_rangeX_end->Name = L"textBox_rangeX_end";
			this->textBox_rangeX_end->ReadOnly = true;
			this->textBox_rangeX_end->Size = System::Drawing::Size(30, 20);
			this->textBox_rangeX_end->TabIndex = 6;
			this->textBox_rangeX_end->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// textBox_rangeY_end
			// 
			this->textBox_rangeY_end->Location = System::Drawing::Point(112, 55);
			this->textBox_rangeY_end->Margin = System::Windows::Forms::Padding(2);
			this->textBox_rangeY_end->Name = L"textBox_rangeY_end";
			this->textBox_rangeY_end->ReadOnly = true;
			this->textBox_rangeY_end->Size = System::Drawing::Size(30, 20);
			this->textBox_rangeY_end->TabIndex = 7;
			this->textBox_rangeY_end->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(155, 58);
			this->label5->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(37, 13);
			this->label5->TabIndex = 8;
			this->label5->Text = L"Size Y";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(155, 36);
			this->label6->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(37, 13);
			this->label6->TabIndex = 9;
			this->label6->Text = L"Size X";
			// 
			// textBox_sizeX
			// 
			this->textBox_sizeX->Location = System::Drawing::Point(196, 33);
			this->textBox_sizeX->Margin = System::Windows::Forms::Padding(2);
			this->textBox_sizeX->Name = L"textBox_sizeX";
			this->textBox_sizeX->ReadOnly = true;
			this->textBox_sizeX->Size = System::Drawing::Size(30, 20);
			this->textBox_sizeX->TabIndex = 10;
			this->textBox_sizeX->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// textBox_sizeY
			// 
			this->textBox_sizeY->Location = System::Drawing::Point(196, 55);
			this->textBox_sizeY->Margin = System::Windows::Forms::Padding(2);
			this->textBox_sizeY->Name = L"textBox_sizeY";
			this->textBox_sizeY->ReadOnly = true;
			this->textBox_sizeY->Size = System::Drawing::Size(30, 20);
			this->textBox_sizeY->TabIndex = 11;
			this->textBox_sizeY->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(32, 80);
			this->label7->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(18, 13);
			this->label7->TabIndex = 12;
			this->label7->Text = L"dx";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(140, 80);
			this->label8->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(18, 13);
			this->label8->TabIndex = 13;
			this->label8->Text = L"dy";
			// 
			// textBox_dx
			// 
			this->textBox_dx->Location = System::Drawing::Point(53, 78);
			this->textBox_dx->Margin = System::Windows::Forms::Padding(2);
			this->textBox_dx->Name = L"textBox_dx";
			this->textBox_dx->ReadOnly = true;
			this->textBox_dx->Size = System::Drawing::Size(66, 20);
			this->textBox_dx->TabIndex = 14;
			this->textBox_dx->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// textBox_dy
			// 
			this->textBox_dy->Location = System::Drawing::Point(162, 78);
			this->textBox_dy->Margin = System::Windows::Forms::Padding(2);
			this->textBox_dy->Name = L"textBox_dy";
			this->textBox_dy->ReadOnly = true;
			this->textBox_dy->Size = System::Drawing::Size(66, 20);
			this->textBox_dy->TabIndex = 15;
			this->textBox_dy->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			// 
			// textBox_outTime
			// 
			this->textBox_outTime->Location = System::Drawing::Point(64, 134);
			this->textBox_outTime->Margin = System::Windows::Forms::Padding(2);
			this->textBox_outTime->Name = L"textBox_outTime";
			this->textBox_outTime->Size = System::Drawing::Size(48, 20);
			this->textBox_outTime->TabIndex = 22;
			this->textBox_outTime->Text = L"0";
			this->textBox_outTime->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->textBox_outTime->TextChanged += gcnew System::EventHandler(this, &mainForm::textBox_outTime_TextChanged);
			// 
			// textBox_calcTime
			// 
			this->textBox_calcTime->Location = System::Drawing::Point(64, 109);
			this->textBox_calcTime->Margin = System::Windows::Forms::Padding(2);
			this->textBox_calcTime->Name = L"textBox_calcTime";
			this->textBox_calcTime->Size = System::Drawing::Size(48, 20);
			this->textBox_calcTime->TabIndex = 21;
			this->textBox_calcTime->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->textBox_calcTime->TextChanged += gcnew System::EventHandler(this, &mainForm::textBox_calcTime_TextChanged);
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(11, 136);
			this->label9->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(50, 13);
			this->label9->TabIndex = 20;
			this->label9->Text = L"Out Time";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(11, 111);
			this->label10->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(51, 13);
			this->label10->TabIndex = 19;
			this->label10->Text = L"CalcTime";
			// 
			// button_applyParameters
			// 
			this->button_applyParameters->Enabled = false;
			this->button_applyParameters->Location = System::Drawing::Point(14, 158);
			this->button_applyParameters->Margin = System::Windows::Forms::Padding(2);
			this->button_applyParameters->Name = L"button_applyParameters";
			this->button_applyParameters->Size = System::Drawing::Size(214, 23);
			this->button_applyParameters->TabIndex = 23;
			this->button_applyParameters->Text = L"Apply Parameters";
			this->button_applyParameters->UseVisualStyleBackColor = true;
			this->button_applyParameters->Click += gcnew System::EventHandler(this, &mainForm::button_applyParameters_Click);
			// 
			// button_startCalc
			// 
			this->button_startCalc->Enabled = false;
			this->button_startCalc->Location = System::Drawing::Point(14, 196);
			this->button_startCalc->Margin = System::Windows::Forms::Padding(2);
			this->button_startCalc->Name = L"button_startCalc";
			this->button_startCalc->Size = System::Drawing::Size(62, 23);
			this->button_startCalc->TabIndex = 24;
			this->button_startCalc->Text = L"Start";
			this->button_startCalc->UseVisualStyleBackColor = true;
			this->button_startCalc->Click += gcnew System::EventHandler(this, &mainForm::button_startCalc_Click);
			// 
			// button_pauseCalc
			// 
			this->button_pauseCalc->Enabled = false;
			this->button_pauseCalc->Location = System::Drawing::Point(93, 196);
			this->button_pauseCalc->Margin = System::Windows::Forms::Padding(2);
			this->button_pauseCalc->Name = L"button_pauseCalc";
			this->button_pauseCalc->Size = System::Drawing::Size(62, 23);
			this->button_pauseCalc->TabIndex = 25;
			this->button_pauseCalc->Text = L"Pause";
			this->button_pauseCalc->UseVisualStyleBackColor = true;
			this->button_pauseCalc->Click += gcnew System::EventHandler(this, &mainForm::button_pauseCalc_Click);
			// 
			// button_stopCalc
			// 
			this->button_stopCalc->Enabled = false;
			this->button_stopCalc->Location = System::Drawing::Point(166, 196);
			this->button_stopCalc->Margin = System::Windows::Forms::Padding(2);
			this->button_stopCalc->Name = L"button_stopCalc";
			this->button_stopCalc->Size = System::Drawing::Size(62, 23);
			this->button_stopCalc->TabIndex = 26;
			this->button_stopCalc->Text = L"Stop";
			this->button_stopCalc->UseVisualStyleBackColor = true;
			this->button_stopCalc->Click += gcnew System::EventHandler(this, &mainForm::button_stopCalc_Click);
			// 
			// menuStrip_main
			// 
			this->menuStrip_main->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->ToolStrip_file});
			this->menuStrip_main->Location = System::Drawing::Point(0, 0);
			this->menuStrip_main->Name = L"menuStrip_main";
			this->menuStrip_main->Padding = System::Windows::Forms::Padding(4, 2, 0, 2);
			this->menuStrip_main->Size = System::Drawing::Size(236, 24);
			this->menuStrip_main->TabIndex = 27;
			this->menuStrip_main->Text = L"menuStrip1";
			// 
			// ToolStrip_file
			// 
			this->ToolStrip_file->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->ToolStrip_file_openMap, 
				this->ToolStrip_file_openConfig});
			this->ToolStrip_file->Name = L"ToolStrip_file";
			this->ToolStrip_file->Size = System::Drawing::Size(37, 20);
			this->ToolStrip_file->Text = L"File";
			// 
			// ToolStrip_file_openMap
			// 
			this->ToolStrip_file_openMap->Name = L"ToolStrip_file_openMap";
			this->ToolStrip_file_openMap->Size = System::Drawing::Size(152, 22);
			this->ToolStrip_file_openMap->Text = L"Open Map";
			this->ToolStrip_file_openMap->Click += gcnew System::EventHandler(this, &mainForm::openToolStripMenuItem_Click);
			// 
			// ToolStrip_file_openConfig
			// 
			this->ToolStrip_file_openConfig->Name = L"ToolStrip_file_openConfig";
			this->ToolStrip_file_openConfig->Size = System::Drawing::Size(152, 22);
			this->ToolStrip_file_openConfig->Text = L"Open Config";
			this->ToolStrip_file_openConfig->Click += gcnew System::EventHandler(this, &mainForm::openConfigToolStripMenuItem_Click);
			// 
			// textBox_isobath
			// 
			this->textBox_isobath->Location = System::Drawing::Point(178, 108);
			this->textBox_isobath->Margin = System::Windows::Forms::Padding(2);
			this->textBox_isobath->Name = L"textBox_isobath";
			this->textBox_isobath->Size = System::Drawing::Size(48, 20);
			this->textBox_isobath->TabIndex = 28;
			this->textBox_isobath->Text = L"-10";
			this->textBox_isobath->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
			this->textBox_isobath->TextChanged += gcnew System::EventHandler(this, &mainForm::textBox_isobath_TextChanged);
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(134, 110);
			this->label11->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(42, 13);
			this->label11->TabIndex = 29;
			this->label11->Text = L"Isobath";
			// 
			// checkBox_autoSaveLayers
			// 
			this->checkBox_autoSaveLayers->AutoSize = true;
			this->checkBox_autoSaveLayers->Location = System::Drawing::Point(118, 136);
			this->checkBox_autoSaveLayers->Margin = System::Windows::Forms::Padding(2);
			this->checkBox_autoSaveLayers->Name = L"checkBox_autoSaveLayers";
			this->checkBox_autoSaveLayers->RightToLeft = System::Windows::Forms::RightToLeft::Yes;
			this->checkBox_autoSaveLayers->Size = System::Drawing::Size(110, 17);
			this->checkBox_autoSaveLayers->TabIndex = 31;
			this->checkBox_autoSaveLayers->Text = L"Auto Save Layers";
			this->checkBox_autoSaveLayers->UseVisualStyleBackColor = true;
			this->checkBox_autoSaveLayers->CheckedChanged += gcnew System::EventHandler(this, &mainForm::checkBox_autoSaveLayers_CheckedChanged);
			// 
			// mainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(236, 230);
			this->Controls->Add(this->checkBox_autoSaveLayers);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->textBox_isobath);
			this->Controls->Add(this->button_stopCalc);
			this->Controls->Add(this->button_pauseCalc);
			this->Controls->Add(this->button_startCalc);
			this->Controls->Add(this->button_applyParameters);
			this->Controls->Add(this->textBox_outTime);
			this->Controls->Add(this->textBox_calcTime);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->textBox_dy);
			this->Controls->Add(this->textBox_dx);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->textBox_sizeY);
			this->Controls->Add(this->textBox_sizeX);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->textBox_rangeY_end);
			this->Controls->Add(this->textBox_rangeX_end);
			this->Controls->Add(this->textBox_rangeY_start);
			this->Controls->Add(this->textBox_rangeX_start);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->menuStrip_main);
			this->KeyPreview = true;
			this->MainMenuStrip = this->menuStrip_main;
			this->Margin = System::Windows::Forms::Padding(2);
			this->MaximizeBox = false;
			this->MaximumSize = System::Drawing::Size(252, 275);
			this->MinimumSize = System::Drawing::Size(252, 249);
			this->Name = L"mainForm";
			this->Text = L"Tsunami";
			this->menuStrip_main->ResumeLayout(false);
			this->menuStrip_main->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private:
		void Invoke_showRealTime(int h, int m, int s)
		{
			if (vf->InvokeRequired)
			{
				showRealTimeDelegate^ d = gcnew showRealTimeDelegate(this, &Wave_renew::mainForm::Invoke_showRealTime);
				this->Invoke(d, h, m, s);
			}
			else
			{
				vf->Text = "Processing, t = " + System::Convert::ToString(h) + "h " + System::Convert::ToString(m) + "m " + System::Convert::ToString(s) + "s";
			}
		}

		void Invoke_updateDraw()
		{
			if (vf->pictureBox_main->InvokeRequired)
			{
				updateDrawDelegate^ d = gcnew updateDrawDelegate(this, &Wave_renew::mainForm::Invoke_updateDraw);
				this->Invoke(d);
			}
			else
			{
				vf->Refresh();
			}
		}

		void Invoke_button_startCalc_changeText(String^ s)
		{
			if (this->button_startCalc->InvokeRequired)
			{
				processCalculationTimeDelegate^ d = gcnew processCalculationTimeDelegate(this, &Wave_renew::mainForm::Invoke_button_startCalc_changeText);
				this->Invoke(d, s);
			}
			else
			{
				this->button_startCalc->Text = s;
			}
		}

		void Invoke_afterCalcDefaults()
		{
			if (this->InvokeRequired)
			{
				generalDefaultsDelegate^ d = gcnew generalDefaultsDelegate(this, &Wave_renew::mainForm::Invoke_afterCalcDefaults);
				this->Invoke(d);
			}
			else
			{
				this->button_startCalc->Enabled = true;
				this->button_pauseCalc->Enabled = false;
				this->button_stopCalc->Enabled = false;
				this->textBox_isobath->Enabled = true;
				this->ToolStrip_file_openMap->Enabled = true;
				this->ToolStrip_file_openConfig->Enabled = true;
				this->button_startCalc->Text = "Start";
			}
		}

		void Invoke_pauseButtonClickDelegate(bool toggled)
		{
			if (this->button_pauseCalc->InvokeRequired)
			{
				pauseButtonDelegate^ d = gcnew pauseButtonDelegate(this, &Wave_renew::mainForm::Invoke_pauseButtonClickDelegate);
				this->Invoke(d, toggled);
			}
			else
			{
				if (toggled)
					this->button_pauseCalc->FlatStyle = FlatStyle::Standard;
				else
					this->button_pauseCalc->FlatStyle = FlatStyle::Popup;
			}
		}

		System::Void button_startCalc_Click(System::Object^  sender, System::EventArgs^  e)
		{
			calculationDelegate^ d = gcnew calculationDelegate(this, &Wave_renew::mainForm::processing);
			calculationThread = gcnew Thread(gcnew ThreadStart(this, &Wave_renew::mainForm::blabla));
			calculationThread->IsBackground = false;
			
			isProcessing = true;

			this->button_applyParameters->Enabled = false;
			this->button_startCalc->Enabled = false;
			this->ToolStrip_file_openMap->Enabled = false;
			this->ToolStrip_file_openConfig->Enabled = false;
			this->textBox_isobath->Enabled = false;
			this->button_pauseCalc->Enabled = true;
			this->button_stopCalc->Enabled = true;

			vf->Show();
			calculationThread->Start();

			isProcessing = false;
		}

		System::Void textBox_calcTime_TextChanged(System::Object^  sender, System::EventArgs^  e)
		{
			this->button_startCalc->Enabled = false; 
			checkReadyForCalculationState();
		}

		System::Void textBox_outTime_TextChanged(System::Object^  sender, System::EventArgs^  e)
		{
			if (this->textBox_outTime->Text != "")
			{
				if (System::Convert::ToDouble(this->textBox_outTime->Text))
				{
					outTime = System::Convert::ToInt32(this->textBox_outTime->Text);
				}
			}
			this->button_startCalc->Enabled = false;
			checkReadyForCalculationState();
		}

		System::Void textBox_isobath_TextChanged(System::Object^  sender, System::EventArgs^  e)
		{
			this->button_startCalc->Enabled = false;
			checkReadyForCalculationState();
		}

		System::Void checkBox_autoSaveLayers_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		{

		}

		System::Void button_applyParameters_Click(System::Object^  sender, System::EventArgs^  e)
		{
			if (!isProcessing)
			{
				this->button_applyParameters->Enabled = false;
				calculationTime = System::Convert::ToInt32(this->textBox_calcTime->Text);
				outTime = System::Convert::ToInt32(this->textBox_outTime->Text);
				isobath = System::Convert::ToDouble(this->textBox_isobath->Text);
				this->button_startCalc->Enabled = true;
			}
			else
			{
				this->button_applyParameters->Enabled = false;
				calculationTime = System::Convert::ToInt32(this->textBox_calcTime->Text);
				outTime = System::Convert::ToInt32(this->textBox_outTime->Text);
			}
		}

		System::Void button_pauseCalc_Click(System::Object^  sender, System::EventArgs^  e)
		{
			if (calculationThread->ThreadState == ThreadState::Suspended)
			{
				calculationThread->Resume();
				this->Invoke_pauseButtonClickDelegate(true);
			}
			else
			{
				calculationThread->Suspend();
				this->Invoke_pauseButtonClickDelegate(false);
			}
		}

		System::Void button_stopCalc_Click(System::Object^  sender, System::EventArgs^  e)
		{
			calculationThread->Abort();
			this->Invoke_afterCalcDefaults();
		}

		System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
			OpenFileDialog^ openMap = gcnew OpenFileDialog;
			openMap->Filter = "*.dat|*.dat|*.mtx|*.mtx";
			openMap->InitialDirectory = ".";
			openMap->RestoreDirectory = true;

			try
			{
				if (openMap->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				{
					mapFileName = openMap->FileName;
				}
				else
					return;

				this->Enabled = false;

				marshal_context^ context = gcnew marshal_context();
				const char* fileExt = context->marshal_as<const char*>(openMap->SafeFileName);
				//char* absfilename = strdup(openMap->SafeFileName);//"C:\temp\file.txt";
				char drive[56], path[56], name[56], ext[56];
				_splitpath(fileExt, drive, path, name, ext);
			
				printf("%s%s", name, ext); // выведет file.txt

				if (strcmp(ext,".dat") == 0) 
				{
					if (this->loadMapDat())
					{
						MessageBox::Show("Map Loading Completed!", "Information!", MessageBoxButtons::OK, MessageBoxIcon::Information);

						if (vf)
							delete vf;
						vf = gcnew ViewForm();
						if (mainBitmap)
							delete mainBitmap;
						this->mainBitmap = gcnew Bitmap(mapSizeX, mapSizeY);
						vf->pictureBox_main->Image = this->mainBitmap;
						mainGraphics = Graphics::FromImage(this->mainBitmap);
						showBottom();
						vf->Show();
						this->Enabled = true;
					}
				}
				else if (strcmp(ext,".mtx") == 0)
				{
					if (this->loadMapMtx())
					{
						MessageBox::Show("Map Loading Completed!", "Information!", MessageBoxButtons::OK, MessageBoxIcon::Information);

						if (vf)
							delete vf;
						vf = gcnew ViewForm();
						if (mainBitmap)
							delete mainBitmap;
						this->mainBitmap = gcnew Bitmap(mapSizeX, mapSizeY);
						vf->pictureBox_main->Image = this->mainBitmap;
						mainGraphics = Graphics::FromImage(this->mainBitmap);
						showBottom();
						vf->Show();
						this->Enabled = true;
					}
				}
				else
					MessageBox::Show("Map Loading Error!", "Error!", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			catch (Exception^ e)
			{
				return;
			}
		}

		System::Void openConfigToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
		{
			OpenFileDialog^ openConfig = gcnew OpenFileDialog;
			openConfig->InitialDirectory = ".";
			openConfig->RestoreDirectory = true;

			try
			{
				String^ configPath = "";
				if (openConfig->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				{
					configPath = openConfig->FileName;
				}
				else
					return;

				XMLConfigReader^ cr = gcnew XMLConfigReader(configPath);
				if (cr->parse())
				{
					MessageBox::Show("Config Loading Completed!", "Information!", MessageBoxButtons::OK, MessageBoxIcon::Information);

					watchingPointsQ = cr->pointsQ;
					point_tmp = cr->points;
					hearthBricksQ = cr->blocksQ;
					terr_tmp = cr->hearth;

					/*
					ofstream out;
					out.open("outttt", ios::out);
					out << cr->pointsQ << "_" << cr->blocksQ << endl;
					out << "hearth" << endl;
					for (int i = 0; i < hearthBricksQ; i++)
					{
						for (int j = 0; j < 10; j++)
							out << terr_tmp[i][j] << "_";
						out << endl;
					}
					out << "points" << endl;
					for (int i = 0; i < watchingPointsQ; i++)
					{
						for (int j = 0; j < 2; j++)
							out << point_tmp[i][j] << "_";
						out << endl;
					}
					out.close();
					*/
				}
				else
					MessageBox::Show("Config Loading Error!", "Error!", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
			catch (Exception^ e)
			{

			}
			
		}

		System::Void mainForm_KeyDown(System::Object^ sender, KeyEventArgs^ e)
		{
			if (e->Control && e->KeyCode == Keys::T)
			{
				test();
			}
		}

		System::Void mainForm_KeyPressed(System::Object^ sender, KeyPressEventArgs^ e)
		{
			e->Handled = true;
		}
	};
}