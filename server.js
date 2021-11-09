//Loading modules
var http = require('http');
var fs = require('fs');
var path = require('path');
var b = require('bonescript');
const execFile = require('child_process').execFile;


// Initialize the server on port 8888
var server = http.createServer(function (req, res) {
    // requesting files
    var file = '.' + ((req.url == '/') ? '/index.html' : req.url);
    var contentType = 'text/html';
    // Uncoment if you want to add css to your web page
    /*
    if(fileExtension == '.css'){
        contentType = 'text/css';
    }*/
    fs.exists(file, function (exists) {
        if (exists) {
            fs.readFile(file, function (error, content) {
                if (!error) {
                    // Page found, write content
                    res.writeHead(200, {
                        'content-type': contentType
                    });
                    res.end(content);
                }
            })
        } else {
            // Page not found
            res.writeHead(404);
            res.end('Page not found');
        }
    })
}).listen(8888);

// Loading socket io module
const { Server } = require("socket.io");
const io = new Server({});
io.on('connection', function (socket) {
    getData(socket)
    socket.on('setLight', handleSetLight);
    socket.on('setWindow', handleSetWindow);
    socket.on('setHeater', handleSetHeater);

    setInterval(getData, 3000, socket);
});

function getRandom(min, max) {
    return (Math.random() * (max - min) + min).toFixed(2);
  }

function getData (socket){
    socket.emit("newMeasurements", { 'light': getRandom(0, 100), 'temp': getRandom(15, 30), 'hum': getRandom(35, 65)} )
}

// Change led state when a button is pressed
function handleSetLight(data) {
    var newData = JSON.parse(data);
    console.log("Light intensity: " + newData.intensity);

    // execFile('./Humidity19', (error, stdout, stderr) => {
    //     if (error) {
    //         console.error('stderr', stderr);
    //         throw error;
    //     }
    //     console.log(stdout);
    // });
}

function handleSetWindow(data) {
    var newData = JSON.parse(data);
    console.log("Set window: " + newData.state);

    // execFile('./Humidity19', (error, stdout, stderr) => {
    //     if (error) {
    //         console.error('stderr', stderr);
    //         throw error;
    //     }
    //     console.log(stdout);
    // });
}

function handleSetHeater(data) {
    var newData = JSON.parse(data);
    console.log("Set heater: " + newData.state);

    // execFile('./Humidity19', (error, stdout, stderr) => {
    //     if (error) {
    //         console.error('stderr', stderr);
    //         throw error;
    //     }
    //     console.log(stdout);
    // });
}

io.listen(server)

// Displaying a console message for user feedback
console.log("Server Running ...")



