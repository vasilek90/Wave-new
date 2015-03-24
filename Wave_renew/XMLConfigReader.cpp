#include "stdafx.h"
#include "XMLConfigReader.h"

XMLConfigReader::XMLConfigReader(String^ path)
{
	reader = gcnew XmlTextReader(path);
}

__Point* XMLConfigReader::XMLRoutine_Point(XmlReader^ _reader)
{
	//Console::WriteLine("----point---");

	__Point* p = new __Point();

	_reader->ReadToFollowing("x");
	_reader->Read();
	p->x = System::Convert::ToDouble(_reader->Value);
	_reader->ReadToFollowing("y");
	_reader->Read();
	p->y = System::Convert::ToDouble(_reader->Value);

	//Console::WriteLine("element {0}_{1}", p->x, p->y);

	//Console::WriteLine("----end point---");

	return p;
}

void XMLConfigReader::XMLRoutine_ObservationPoints(XmlReader^ _reader)
{
	//Console::WriteLine("----observationpoints---");

	_reader->Read();
	_reader->MoveToAttribute("count");
	pointsQ = System::Convert::ToInt32(_reader->Value);
	points = new Double*[pointsQ];
	for (int i = 0; i < pointsQ; i++)
	{
		points[i] = new Double(3);
	}
	for (int i = 0; i < pointsQ; i++)
	{
		_reader->ReadToFollowing("Point");
		__Point* p = XMLRoutine_Point(_reader->ReadSubtree());
		points[i][0] = p->x;
		points[i][1] = p->y;
		points[i][2] = 0.0;
		delete p;
	}

	//Console::WriteLine("----end observationpoints---");
}

void XMLConfigReader::XMLRoutine_Step(XmlReader^ _reader, int blockn)
{
	//Console::WriteLine("----step---");

	_reader->ReadToFollowing("StartTime");
	_reader->Read();

	_reader->ReadToFollowing("FinishTime");
	_reader->Read();
	hearth[blockn][0] = System::Convert::ToInt32(_reader->Value);

	_reader->ReadToFollowing("Height");
	_reader->Read();
	hearth[blockn][1] = System::Convert::ToInt32(_reader->Value);

	//Console::WriteLine("element {0}_{1}", hearth[blockn][0], hearth[blockn][1]);

	//Console::WriteLine("----end step---");
}

void XMLConfigReader::XMLRoutine_Brick(XmlReader^ _reader, int blockn)
{
	//Console::WriteLine("----brick---");

	_reader->ReadToFollowing("Coordinates");
	for (int i = 0; i < 4; i++)
	{
		_reader->ReadToFollowing("Point");
		__Point* p = XMLRoutine_Point(_reader->ReadSubtree());
		hearth[blockn][2 * (i + 1)] = p->x;
		hearth[blockn][2 * (i + 1) + 1] = p->y;
		delete p;
	}
	_reader->ReadToFollowing("Scenario");
	_reader->MoveToAttribute("stepq");
	int stepq = System::Convert::ToInt32(_reader->Value);
	for (int i = 0; i < stepq; i++)
	{
		_reader->ReadToFollowing("Step");
		XMLRoutine_Step(_reader->ReadSubtree(), blockn);
	}

	//Console::WriteLine("----end brick---");
}

void XMLConfigReader::XMLRoutine_Bricks(XmlReader^ _reader)
{
	//Console::WriteLine("----bricks---");

	_reader->Read();
	_reader->MoveToAttribute("count");
	blocksQ = System::Convert::ToInt32(_reader->Value);
	hearth = new Double*[blocksQ];
	for (int i = 0; i < blocksQ; i++)
	{
		hearth[i] = new Double[10];
	}
	for (int i = 0; i < blocksQ; i++)
	{
		_reader->ReadToFollowing("Brick");
		XMLRoutine_Brick(_reader->ReadSubtree(), i);
	}

	//Console::WriteLine("----end bricks---");
}

bool XMLConfigReader::parse()
{	
	try
	{
		//Console::WriteLine("start reading");
		while (reader->Read())
		{
			switch (reader->NodeType)
			{
			case XmlNodeType::Element:
				if (reader->Name->ToLower() == "observationpoints")
				{
					XmlReader^ r = reader->ReadSubtree();
					XMLRoutine_ObservationPoints(r);
				}
				if (reader->Name->ToLower() == "bricks")
				{
					XmlReader^ r = reader->ReadSubtree();
					XMLRoutine_Bricks(r);
				}
			}
		}
		//Console::WriteLine("----end reading---");

		return true;
	}
	catch (Exception^ e)
	{
		return false;
	}
}
