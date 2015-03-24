#pragma once

#include<iostream>

using namespace System;
using namespace System::Xml;

struct __Point
{
	Double x;
	Double y;
};

ref class XMLConfigReader
{
public:
	XMLConfigReader(String^);
	bool parse();

public:
	Double** points;
	Double** hearth;
	int pointsQ;
	int blocksQ;

private:
	XmlReader^ reader;
	void XMLRoutine_Bricks(XmlReader^ reader);
	void XMLRoutine_Brick(XmlReader^ reader, int blockn);
	void XMLRoutine_Step(XmlReader^ reader, int blockn);
	void XMLRoutine_ObservationPoints(XmlReader^ reader);
	__Point* XMLRoutine_Point(XmlReader^ reader);
};

