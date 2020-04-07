

// Initialize express and server
var express = require('express'),
	app = express(),
	fs = require('fs'),
        os = require('os'),
	server = require('http').createServer(app),
	io = require('socket.io').listen(server),
        SerialPort = require("serialport");



//init for SerialPort connected to Arduino
var serialPort = new SerialPort('/dev/ttyACM0',
    {   baudRate: 9600,
        dataBits: 8,
        parity: 'none',
        stopBits: 1,
        flowControl: false
    });
/*
serialPort.on("open", function () {
    console.log('serialPort open');
    serialPort.write("LEDOFF\n");
});
*/
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
//var commonStatus = 'ON';
server.listen(4050);

// Set '/public' as the static folder. Any files there will be directly sent to the viewer
app.use(express.static(__dirname + '/public'));

// Set index.html as the base file
app.get('/', function (req, res) {
  	res.sendfile(__dirname + '/index.html');
});





io.sockets.on('connection', function (socket) {

       //Set the current common status to the new client
        socket.emit('randy connected', { command: 'Connected' });
     
   	// When I've received 'robot command' message from this connection...
	  socket.on('randy command', function (data) {
	    console.log(data);
	    var command = data.command;
              switch  (command)
	    {

           case  'forward' :
	          console.log("Forward");
	          //commonStatus = 'OFF';
	         //serialPort.write("LEDON\n");
              break;
			  
           case  'left' :
	          console.log("Left");
	          //commonStatus = 'OFF';
	         //serialPort.write("LEDON\n");
              break;
			  
           case  'center' :
	          console.log("Center");
	          //commonStatus = 'OFF';
	         //serialPort.write("LEDON\n");
              break;
			  
           case  'right' :
	          console.log("Right");
	          //commonStatus = 'OFF';
	         //serialPort.write("LEDON\n");
              break;
			  
           case  'power' :
	          console.log("Power");
	          //commonStatus = 'OFF';
	         //serialPort.write("LEDON\n");
              break;	
			  
           case  'forward' :
	          console.log("ON-> OFF");
	          //commonStatus = 'OFF';
	         //serialPort.write("LEDON\n");
              break;			  
	     
	    }  //switches    
	    
         });

    //Info all clients if this client disconnect
    socket.on('disconnect', function () {
           });
});




