#include <QCoreApplication>

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMetaEnum>
#include <QCoreApplication>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTextStream>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_PORT 32



//Close 1
//0x01 0x05 0x00 0x00 0xFF 0x00 0x8C 0x3A
//Open 1
//0x01 0x05 0x00 0x00 0x00 0x00 0xCD 0xCA

//Close 2
//0x01 0x05 0x00 0x01 0xFF 0x00 0xDD 0xFA
//Open 2
//0x01 0x05 0x00 0x01 0x00 0x00 0x9C 0x0A

//Close 3
//0x01 0x05 0x00 0x02 0xFF 0x00 0x2D 0xFA
//Open 3
//0x01 0x05 0x00 0x02 0x00 0x00 0x6C 0x0A

//Close 4
//0x01 0x05 0x00 0x03 0xFF 0x00 0x7C 0x3A
//Open 4
//0x01 0x05 0x00 0x03 0x00 0x00 0x3D 0xCA


uint8_t cmd_sw1_close[] = {0x01, 0x05, 0x00, 0x00, 0xFF, 0x00, 0x8C, 0x3A};
uint8_t cmd_sw1_open[]  = {0x01, 0x05, 0x00, 0x00, 0x00, 0x00, 0xCD, 0xCA};

uint8_t cmd_sw2_close[] = {0x01, 0x05, 0x00, 0x01, 0xFF, 0x00, 0xDD, 0xFA};
uint8_t cmd_sw2_open[]  = {0x01, 0x05, 0x00, 0x01, 0x00, 0x00, 0x9C, 0x0A};

uint8_t cmd_sw3_close[] = {0x01, 0x05, 0x00, 0x02, 0xFF, 0x00, 0x2D, 0xFA};
uint8_t cmd_sw3_open[]  = {0x01, 0x05, 0x00, 0x02, 0x00, 0x00, 0x6C, 0x0A};

uint8_t cmd_sw4_close[] = {0x01, 0x05, 0x00, 0x03, 0xFF, 0x00, 0x7C, 0x3A};
uint8_t cmd_sw4_open[]  = {0x01, 0x05, 0x00, 0x03, 0x00, 0x00, 0x3D, 0xCA};




int main(int argc, char *argv[])
{
    QTextStream out(stdout);
    const auto serialPortInfos = QSerialPortInfo::availablePorts();

    const QString blankString = "N/A";
    QSerialPortInfo port_list[MAX_PORT];

    out << "Total number of ports available: " << serialPortInfos.count() << endl;
    int counter = 0;
    for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
        port_list[counter] = serialPortInfo;

        out << endl
            << "[" << counter << "]" << " Port: " << serialPortInfo.portName() << endl
            << "    Location: " << serialPortInfo.systemLocation() << endl
            << "    Description: " << (!serialPortInfo.description().isEmpty() ? serialPortInfo.description() : blankString) << endl
            << "    Manufacturer: " << (!serialPortInfo.manufacturer().isEmpty() ? serialPortInfo.manufacturer() : blankString) << endl
            << "    Serial number: " << (!serialPortInfo.serialNumber().isEmpty() ? serialPortInfo.serialNumber() : blankString) << endl
            << "    Vendor Identifier: " << (serialPortInfo.hasVendorIdentifier()
                                             ? QByteArray::number(serialPortInfo.vendorIdentifier(), 16)
                                             : blankString) << endl
            << "    Product Identifier: " << (serialPortInfo.hasProductIdentifier()
                                              ? QByteArray::number(serialPortInfo.productIdentifier(), 16)
                                              : blankString) << endl
            << "    Busy: " << (serialPortInfo.isBusy() ? "Yes" : "No") << endl;

        counter++;
    }

    printf("Please select Serial Port no [0-%d]: ", counter-1);
    int choice;
    scanf("%d", &choice);


    QSerialPort serialPort;
    uint8_t* data;
    QByteArray readData;
    char buffer[32];
    int buffer_p;
    serialPort.setPortName(port_list[choice].portName());

    int serialPortBaudRate = 9600;
    serialPort.setBaudRate(serialPortBaudRate);

    if (!serialPort.open(QIODevice::ReadWrite)) {
        printf("Open Failed!\n");
        return 0;
    }
    else {
        printf("Open Success!\n");
    }
    
    
    while(1)
    {
    	printf("Available Command:\n");
    	printf("   1. SW1 Close.\n");
    	printf("   2. SW1 Open.\n");
    	printf("   3. SW2 Close.\n");
    	printf("   4. SW2 Open.\n");
    	printf("   5. SW3 Close.\n");
    	printf("   6. SW3 Open.\n");
    	printf("   7. SW4 Close.\n");
    	printf("   8. SW4 Open.\n");
    	printf("   0. Exit.\n");
	    printf("Please select Action [0-8]: ");
	    
	    int input;
	    scanf("%d", &input);
	    
	    char* cmd;
	    
	    if(input == 0)
	    {
	    	break;
	    } 
	    else if(input == 1)
	    {
	    	cmd = (char*)cmd_sw1_close;
	    }
	    else if(input == 2)
	    {
	    	cmd = (char*)cmd_sw1_open;
	    }
	    else if(input == 3)
	    {
	    	cmd = (char*)cmd_sw2_close;
	    }
	    else if(input == 4)
	    {
	    	cmd = (char*)cmd_sw2_open;
	    }
	    else if(input == 5)
	    {
	    	cmd = (char*)cmd_sw3_close;
	    }
	    else if(input == 6)
	    {
	    	cmd = (char*)cmd_sw3_open;
	    }
	    else if(input == 7)
	    {
	    	cmd = (char*)cmd_sw4_close;
	    }
	    else if(input == 8)
	    {
	    	cmd = (char*)cmd_sw4_open;
	    }
	    else
	    {
	    	printf("Unrecognized input %d!\n", input);
	    	continue;
	    }

        printf("CMD Sent!\n");
        while(1)
        {
            serialPort.waitForReadyRead(10000);
            readData = serialPort.readAll();

            printf("Recv %d byte: ", readData.size());
            data = (uint8_t*)readData.data();
            for(int i=0;i<readData.size();i++)
            {
                printf("0x%x ", data[i]);
            }
            printf("\n");

            if(buffer_p >= 8)
                break;
        }
    }


    {
        ;
        uint8_t cmd[] =
        {
            0x01, 0x05, 0x00, 0x00, 0xFF, 0x00, 0x8C, 0x3A,
        };
        serialPort.write((const char*)cmd, sizeof(cmd));
        printf("Open1 Sent!\n");

        while(1)
        {
            serialPort.waitForReadyRead(10000);
            readData = serialPort.readAll();

            printf("Recv %d byte: ", readData.size());
            data = (uint8_t*)readData.data();
            for(int i=0;i<readData.size();i++)
            {
                printf("0x%x ", data[i]);
            }
            printf("\n");

            if(buffer_p >= 8)
                break;
        }

        printf("Open1 Success!\n");
    }


    return 0;
}
