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

#define MAX_PORT 32

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

    int serialPortBaudRate = 115200;
    serialPort.setBaudRate(serialPortBaudRate);

    if (!serialPort.open(QIODevice::ReadWrite)) {
        printf("Open Failed!\n");
        return 0;
    }
    else {
        printf("Open Success!\n");
    }

    buffer_p = 0;
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

    }



    return 0;
}
