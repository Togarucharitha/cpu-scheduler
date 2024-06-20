const express = require('express');
const bodyParser = require('body-parser');
const { execFile } =require('child_process').execFile;
const path = require('path');
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

    console.log("Received data:", req.body);

    const outfile = "a.exe";
    const command = [schedulerType, timeQuantum, ...processes.map(p => JSON.stringify(p))];


    let executable = `./${outfile}`;
    execFile(executable, ['-arg1'], null,(err, stdout, stderr) => {
      if (err) {
        console.log(err);
      } else {
        execFile(2, [command], { shell: true }, (err, stdout, stderr) => {
          if (err) {
            console.log(err);
          } else {
            console.log(`what is printed to the console: ${stdout}`);
          }
        });
      }
    })
  
});

 

app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});
