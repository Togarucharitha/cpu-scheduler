const express = require('express');
const bodyParser = require('body-parser');
const { exec } = require('child_process');
const path = require('path');
const os = require('os');

const app = express();
const PORT = 3000; // or any available port

// Middleware
app.use(bodyParser.json());
app.use(express.static('public'));

// Serve HTML file
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'Html.html'));
});

// Endpoint to handle scheduling requests
app.post('/schedule', (req, res) => {
    const { schedulerType, timeQuantum, processes } = req.body;

    const processesWithNumbers = processes.map(p => ({
        processId: Number(p.processId),
        arrivalTime: Number(p.arrivalTime),
        burstTime: Number(p.burstTime)
    }));

    console.log("Received data:", req.body);
    const processesJson = JSON.stringify(processesWithNumbers);


    // Properly escape the JSON string for the command-line argument
    const escapedProcessesJson = processesJson.replace(/"/g, '\\"');

    // Construct command based on the operating system
    console.log("schedulerType ", schedulerType, "timequantum", timeQuantum, "processes", processes);
    let command;
    if (os.platform() === 'win32') {
        command = `a.exe ${schedulerType} ${timeQuantum} "${escapedProcessesJson}"`;
    } else {
        command = `./a ${schedulerType} ${timeQuantum} "${escapedProcessesJson}"`;
    }
    console.log("Executing command:", command);

    exec(command, (error, stdout, stderr) => {
        if (error) {
            console.error(`exec error: ${error}`);
            res.status(500).send(`Error: ${error.message}`);
            return;
        }
        console.log("Command output:", stdout);
        res.send(stdout);
    });
});

app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});
