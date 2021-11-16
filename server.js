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
const {
    Server
} = require("socket.io");

const io = new Server({});


let readData = {
    light: 0,
    hum: 0,
    temp: 0,
    windowState: 0,
    heaterState: 0,
    artificialLightIntensity: 0
}

io.on('connection', function (socket) {
    getData(socket)
    socket.on('setLight', handleSetLight);
    socket.on('setWindow', handleSetWindow);
    socket.on('setHeater', handleSetHeater);

    setInterval(getData, 5000, socket);
});


function getLight(){
    // 1000 - 3500

    execFile('./scripts/LDRRead.sh', (error, stdout, stderr) => {
        if (error) {
            console.error('stderr', stderr);
            throw error;
        }
        result = ((stdout - 1000) / 25) + ''
        console.log('light: ' + result);
        readData.light = result
    })
}

function getTemp(){
    execFile('./scripts/humidity', ['temp'], (error, stdout, stderr) => {
        if (error) {
            console.error('stderr', stderr);
            throw error;
        }
        console.log(stdout);
        readData.temp = stdout
    })
}

function getHumidity(){
    execFile('./scripts/humidity', ['hum'], (error, stdout, stderr) => {
        if (error) {
            console.error('stderr', stderr);
            throw error;
        }
        console.log('hum: ' + stdout);
        readData.hum = stdout
    })
}

function getWindowState(){
    execFile('./scripts/windowRead.sh', (error, stdout, stderr) => {
        if (error) {
            console.error('stderr', stderr);
            throw error;
        }
        result = stdout == 1000000 ? 0 : 1
        console.log('window ' + result);
        readData.windowState = result
    })
}

function getHeaterState(){
    execFile('./scripts/heaterRead.sh', (error, stdout, stderr) => {
        if (error) {
            console.error('stderr', stderr);
            throw error;
        }
        console.log('heater ' + stdout);
        readData.heaterState = stdout
    }) 
}

function getArtificialLightIntensity(){
    execFile('./scripts/LEDRead.sh', (error, stdout, stderr) => {
        if (error) {
            console.error('stderr', stderr);
            throw error;
        }
        dc = stdout
        percent = (dc * 100) / 20000000
        console.log('art light: ' + percent + '%');
        readData.artificialLightIntensity = percent
    }) 
}


function getData(socket) {
    getTemp()
    getLight()
    getWindowState()
    getHeaterState()
    getArtificialLightIntensity()
    getHumidity()

    socket.emit("newMeasurements", readData)
}



// Change led state when a button is pressed
function handleSetLight(data) {
    var parsedData = JSON.parse(data);
    console.log("Light intensity: " + parsedData.intensity);
    setLight(parsedData.intensity)
}

function setLight(intensity) {
    // execute('LEDControl', [intensity])
    execFile('./scripts/LEDControl', [intensity], (error, stdout, stderr) => {
        if (error) {
            console.error('stderr', stderr);
            throw error;
        }
        console.log(stdout);
    })
}

function handleSetWindow(data) {
    var parsedData = JSON.parse(data);
    console.log("Set window: " + parsedData.state);

    setServo(parsedData.state)
}

function setServo(position) {
    // 0 - 1
    // execute('a.out', [position])
    execFile('./scripts/a.out', [position], (error, stdout, stderr) => {
        if (error) {
            console.error('stderr', stderr);
            throw error;
        }
        console.log(stdout);
    })
}

function handleSetHeater(data) {
    console.log(data)
    var parsedData = JSON.parse(data);
    console.log("Set heater: " + parsedData.state);

    setHeater(parsedData.state)
}

function setHeater(state){

    // execute('heaterWrite.sh', [state])
    execFile('./scripts/heaterWrite.sh', [state], (error, stdout, stderr) => {
        if (error) {
            console.error('stderr', stderr);
            throw error;
        }
        console.log(stdout);
    })
}

io.listen(server)

// Displaying a console message for user feedback
console.log("Server Running ...")


function execute(script, args) {
    if (!args)
        args = []
    execFile('./scripts/' + script, args, (error, stdout, stderr) => {
        if (error) {
            console.error('stderr', stderr);
            throw error;
        }
        console.log(stdout);
    })
}