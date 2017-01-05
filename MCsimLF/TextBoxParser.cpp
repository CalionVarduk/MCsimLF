#include "TextBoxParser.h"

using namespace MCsimLF::Helpers;

Int32 TextBoxParser::tryInt32(TextBox^ box)
{
	Int32 result;
	Int32::TryParse(box->Text, result);
	box->Text = result.ToString();
	return result;
}

Int32 TextBoxParser::tryInt32(TextBox^ box, Int32 min, Int32 max)
{
	Int32 result;
	Int32::TryParse(box->Text, result);
	result = Limiter<Int32>::minMaxCheck(result, min, max);
	box->Text = result.ToString();
	return result;
}

UInt32 TextBoxParser::tryUInt32(TextBox^ box)
{
	UInt32 result;
	UInt32::TryParse(box->Text, result);
	box->Text = result.ToString();
	return result;
}

UInt32 TextBoxParser::tryUInt32(TextBox^ box, UInt32 min, UInt32 max)
{
	UInt32 result;
	UInt32::TryParse(box->Text, result);
	result = Limiter<UInt32>::minMaxCheck(result, min, max);
	box->Text = result.ToString();
	return result;
}

float TextBoxParser::tryFloat(TextBox^ box)
{
	float result;
	Single::TryParse(box->Text, result);
	box->Text = result.ToString();
	return result;
}

float TextBoxParser::tryFloat(TextBox^ box, float min, float max)
{
	float result;
	Single::TryParse(box->Text, result);
	result = Limiter<float>::minMaxCheck(result, min, max);
	box->Text = result.ToString();
	return result;
}

double TextBoxParser::tryDouble(TextBox^ box)
{
	double result;
	Double::TryParse(box->Text, result);
	box->Text = result.ToString();
	return result;
}

double TextBoxParser::tryDouble(TextBox^ box, double min, double max)
{
	double result;
	Double::TryParse(box->Text, result);
	result = Limiter<double>::minMaxCheck(result, min, max);
	box->Text = result.ToString();
	return result;
}

Int32 TextBoxParser::tryInt32(TitledTextBox^ box)
{
	Int32 result;
	Int32::TryParse(box->Text, result);
	box->Text = result.ToString();
	return result;
}

Int32 TextBoxParser::tryInt32(TitledTextBox^ box, Int32 min, Int32 max)
{
	Int32 result;
	Int32::TryParse(box->Text, result);
	result = Limiter<Int32>::minMaxCheck(result, min, max);
	box->Text = result.ToString();
	return result;
}

UInt32 TextBoxParser::tryUInt32(TitledTextBox^ box)
{
	UInt32 result;
	UInt32::TryParse(box->Text, result);
	box->Text = result.ToString();
	return result;
}

UInt32 TextBoxParser::tryUInt32(TitledTextBox^ box, UInt32 min, UInt32 max)
{
	UInt32 result;
	UInt32::TryParse(box->Text, result);
	result = Limiter<UInt32>::minMaxCheck(result, min, max);
	box->Text = result.ToString();
	return result;
}

float TextBoxParser::tryFloat(TitledTextBox^ box)
{
	float result;
	Single::TryParse(box->Text, result);
	box->Text = result.ToString();
	return result;
}

float TextBoxParser::tryFloat(TitledTextBox^ box, float min, float max)
{
	float result;
	Single::TryParse(box->Text, result);
	result = Limiter<float>::minMaxCheck(result, min, max);
	box->Text = result.ToString();
	return result;
}

double TextBoxParser::tryDouble(TitledTextBox^ box)
{
	double result;
	Double::TryParse(box->Text, result);
	box->Text = result.ToString();
	return result;
}

double TextBoxParser::tryDouble(TitledTextBox^ box, double min, double max)
{
	double result;
	Double::TryParse(box->Text, result);
	result = Limiter<double>::minMaxCheck(result, min, max);
	box->Text = result.ToString();
	return result;
}