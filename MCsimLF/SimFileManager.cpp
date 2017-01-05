#include "SimFileManager.h"

using namespace MCsimLF::FileManip;

SimFileManager::SimFileManager()
{
	_cipher = gcnew CVAwfulCipher();
	_cipher->setKey(_initKey());
}

bool SimFileManager::save(String^ path, array<Byte>^ monteCarlo)
{
	array<Byte>^ data = HuffmanCompression::compress(monteCarlo);
	_cipher->adjustBeforeEncryption(data);
	data = _cipher->encrypt(data);

	if(data->Length == 0)
		return false;

	FileStream^ _stream = gcnew FileStream(path, FileMode::Create);
	BinaryWriter^ bWriter = gcnew BinaryWriter(_stream);
	bWriter->Write(_getSignature());
	bWriter->Write(data);
	bWriter->Close();

	return true;
}

array<Byte>^ SimFileManager::load(String^ path)
{
	FileStream^ _stream = gcnew FileStream(path, FileMode::Open);
	BinaryReader^ bReader = gcnew BinaryReader(_stream);
	Int32 sigLength = _getSignature()->Length;
	
	if(bReader->BaseStream->Length >= sigLength) {
		array<Byte>^ signature = bReader->ReadBytes(sigLength);
		array<Byte>^ data = bReader->ReadBytes((Int32)bReader->BaseStream->Length - 4);
		bReader->Close();

		if(_isSignatureCorrect(signature)) {
			data = _cipher->decrypt(data);
			_cipher->adjustAfterDecryption(data);
			data = HuffmanCompression::decompress(data);
			return data;
		}
	}
	return gcnew array<Byte>(0);
}

bool SimFileManager::_isSignatureCorrect(array<Byte>^ toCheck)
{
	array<Byte>^ sig = _getSignature();
	for(Int32 i = 0; i < sig->Length; ++i)
		if(toCheck[i] != sig[i]) return false;
	return true;
}

array<Byte>^ SimFileManager::_getSignature()
{
	array<Byte>^ sig = gcnew array<Byte>(4); // CVMC
	sig[0] = 67; sig[1] = 86; sig[2] = 77; sig[3] = 67;
	return sig;
}

array<Byte>^ SimFileManager::_initKey()
{
	String^ keyString = "Czas utworzenia klucza: 2015-11-16 19:08;" + //41
						"Autor algorytmu szyfrujacego i wlasciciel klucza: Lukasz Furlepa (aka CalionVarduk);" + //84
						"Nazwa aplikacji: Monte Carlo Simulation~;" + //41
						"Aplikacja stworzona na przedmiot 'Symulacje i modelowanie komputerowe', Uniwersytet Slaski"; //90

	array<Byte>^ key = gcnew array<Byte>(keyString->Length);
	for(Int32 i = 0; i < key->Length; ++i)
		key[i] = (Byte)(keyString[i] + (3 * i));

	keyString = "";
	return key;
}