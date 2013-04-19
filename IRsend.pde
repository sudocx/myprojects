#include <SPI.h>
#include <Ethernet.h>
#include <IRremote.h>

#define BAUT_RATE 9600

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 10 ,120 ,137 ,134 };
byte gateway[] = { 10 ,120 ,137 ,1 };
byte subnet[] = { 255 ,255 ,255 ,0 };
Server server(80); 

String readString = String(30);

IRsend _infrared_sender;


void setup(){
    server.begin();
    Ethernet.begin(mac, ip, gateway, subnet);
    delay(1000);
    //pinMode(4, OUTPUT);
    //digitalWrite(4, LOW);
    Serial.begin(BAUT_RATE);
} 

void loop(){
    Client client = server.available();
    if (client) {
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                if (readString.length() < 30)
                {
                    readString = readString + c;
                }
                if (c == '\n') {
                    Serial.print(readString);
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println();


                    if(readString.startsWith("GET /?out=4&status=1"))
                    {Serial.print("\n 4 HIGH \n");
                        //digitalWrite(4,HIGH);
			_infrared_sender.send_ir_data(TCL, 213); //add by cx
                        client.print("{\"status\" : \"1\" , \"out\" : \"");
                        client.print(4);
                        client.print("\"}");
                    }

                    if(readString.startsWith("GET /?out=4&status=0"))
                    {Serial.print("\n 4 LOW \n");
                        //digitalWrite(4, LOW);
			_infrared_sender.send_ir_data(TCL, 213); //add by cx
                        client.print("{\"status\" : \"0\" , \"out\" : \"");
                        client.print(4);
                        client.print("\"}");
                    }

                    if(readString.startsWith("GET /?out=all"))
                    {
                        Serial.print("\n OUT ALL\n");
                        client.print("{\"ip\" : \"10.120.137.134\", ");
                        client.print("\"devices\" : ");
                        client.print("[{ \"type\" : \"light\", \"name\" : \"LED_RED\", \"out\" : \"");
                        client.print("4");
                        client.print("\"}");
                        client.print("]}");
                    }
                    readString="";
                    client.stop();
                }
            }
        }
    }
}


