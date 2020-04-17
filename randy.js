/*
Hi-Techno Barrio
by:
  Christopher Coballes
*/

// Initialize express and server
var express = require('express'),
	app = express(),
	fs = require('fs'),
        os = require('os'),
	server = require('http').createServer(app),
	io = require('socket.io').listen(server),
        arduinoMessage = '',
        receivedData = "";
        SerialPort = require("serialport");


/*
var serialport = require('serialport'); // include the library
var SerialPort = serialport.SerialPort; // make a local instance
var portName = process.argv[2]; // get port name from the command line:
var sp = new SerialPort(portName, {  */

//var Readline = SerialPort.parsers.Readline;


//init for SerialPort connected to Arduino
var serialPort = new SerialPort('/dev/ttyACM0',
    {   baudRate: 115200,
        dataBits: 8,
        parity: 'none',
        stopBits: 1,
        flowControl: false  //
       // parser: new serialPort.parsers.Readline("\n")
    });

//Debug Serial Messages
 var serialDebugger = function(buffer, socket) {
    // concatenating the string buffers sent via usb port
    arduinoMessage += buffer.toString();

    // detecting the end of the string
    if (arduinoMessage.indexOf('\r') >= 0) {
      // log the message into the terminal
         console.log(arduinoMessage);
      // send the message to the client
      //socket.volatile.emit('notification', arduinoMessage); // going to web page!
      // reset the output string to an empty value
      arduinoMessage = '';
    }
  };

//Display my IP
var networkInterfaces=os.networkInterfaces();

for (var interface in networkInterfaces) {

    networkInterfaces[interface].forEach(
        function(details){

            if (details.family=='IPv4'
                && details.internal==false) {
                    console.log(interface, details.address);
        }
    });
}

//All clients have a common status
server.listen(4050);

// Set '/public' as the static folder. Any files there will be directly sent to the viewer
app.use(express.static(__dirname + '/public'));

// Set index.html as the base file
app.get('/', function (req, res) {
  	res.sendfile(__dirname + '/index.html');
});


var robot_drive_power = 14 ;
// CTRL: control, SLCT: slector,PWR: power,ENDR: 

function sendPacket( CTRL,SLCT,PWR,ENDR)
{
   var PACKET_SIZE = 4;
   var packet = new Uint8Array( PACKET_SIZE );
   packet[0] = CTRL; // Byte code for robot control ( 'r' in ASCII )
   packet[1] = SLCT; // f in ASCII
   packet[2] = PWR   // integer as value of PWM
   packet[3] = ENDR; // NULL

// Send packet over serial
   console.log('Sending Packet');
   serialPort.write( packet );
    for( i = 0; i < PACKET_SIZE; i++ ) {
	console.log( packet[i] );
		}
	console.log('Packet Sent');
}


io.sockets.on('connection', function (socket) {

       //Set the current common status to the new client
        socket.emit('randy connected', { command: 'Connected' });
     
   	// When I've received 'robot command' message from this connection...
	  socket.on('randy command', function (data) {
	    console.log(data);
	    var command = data.command;
            robot_drive_power = data.power;
		
           switch  (command)
	    {

           case  'forward' :
	          console.log("Forward");
                  sendPacket( 114,102,robot_drive_power,0); // f in ASCII
                  break;
	  
           case  'left' :
	          console.log("Left");
                        sendPacket( 114,108,robot_drive_power,0);// l in ASCII
                   break;
			  
           case  'center' :
	          console.log("Center");
	                 sendPacket( 114,99,robot_drive_power,0); // c in ASCII
		         break;
			  
           case  'right' :
	          console.log("Right");
                        sendPacket( 114,114,robot_drive_power,0);// r in ASCII
                        break;
			  
		 case 'backward':  
			sendPacket( 114,98,robot_drive_power,0) ;// b in ASCII
                        break;
           case  'stop' :
	          console.log("Stop");
	                 sendPacket( 114,115,robot_drive_power,0) ;// s in ASCII
                  break;	
			  
          case 'set_power':
		    	// javascript is handling the command and just passing it to node.js
                        sendPacket( 114,115,robot_drive_power,0) ;// s in ASCII
			break;

	  case 'LED': 
                   console.log("Stop");
                      sendPacket( 108,data.red,data.green,data.blue) ;
		      break;	  
	  
	    }  //switches    
		
	    
         });

          //Info all clients if this client disconnect
           socket.on('disconnect', function () {
           });
});

  // listen all the serial port messages sent from arduino and passing them to the proxy function sendMessage
     serialPort.on('data', function(data) { 
	         receivedData += data.toString();
                  console.log("Here!" + data);
                  receivedData = '';
		  });  

//var parser = new Readline();
// serialPort.pipe(parser);
// parser.on('data', function (data) {
// console.log('data received: ' + data)
//})



