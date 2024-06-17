=================================== Execution and Accesss ============================================

a exe file named : A-7_FDA.exe ( With a icon ) can be found in the Root Directory exe file will work as a standalone.
simply clicking on it will start the application.

The Link to the source code itself : https://github.com/harsh-agr79/cpelectron (does not contain the final packaged folder or executable file)

If the source code is accessed through github.
1. The device needs to be installed with Node.js from the web
2. after cloning or downloading the source code the following commands needs to be executes in the terminal
 ==>  "npm i"
3. After which to Execute the The program run.bat file is to be clicked which will start the application

Refer Video in case of confusion.

============================ Source Code, Access and files Breakdown =================================

The provided folder is packaged folder exported after the complete development of the project.

The Link to the source code itself : https://github.com/harsh-agr79/cpelectron

Reason for seperate final packaged folder and source code:
As during development phase Node.js is requried to create the environment to run the application, after exporting the source code as a packaged folder it can work in any device as a standalone application capable of running which will not require the user to download any extra dependencies or applications, and would not require to run any node scripts or command as well.

Although a standalone package is created during the export, it is only required for the frontend part of the project the backend part of the project remains intact and it is included along with the application in the directory /resources/app. Which can be individually accessed and checked. The directory also contains all the txt files used for data storage and the frontend part of the project which contain HTML,CSS,Javascript files. And the mapping of important files and folders have been provided below.


To check if the backend.c is really being executed the second method of running the source code is to be executed( By installing Node and running the run.bat file ). As the application runs, we can check terminal inputs and outputs from the backend that the frontend is accessing on terminal that the application is running on.


Commands Run by the run.bat file are:

```
    @echo off
    gcc backend.c -o backend
    if %errorlevel% equ 0 (
      npm run watch
    ) else (
      echo Compilation failed. Not running npm script.
    )
```

Directory Distribution of the packaged final Application.

The Main Backend Source Code files for the backend C part of the project are in the directory /resources/app.
The directory also contains all the txt files used for data storage and the frontend part of the project which contain HTML,CSS,Javascript files.

        A-7_FDA-win32-x64
        ├── .gitattributes
        ├── A-7_FDA.exe
        ├── chrome_100_percent.pak
        ├── chrome_200_percent.pak
        ├── d3dcompiler_47.dll
        ├── directory-structure.txt
        ├── ffmpeg.dll
        ├── icudtl.dat
        ├── libEGL.dll
        ├── libGLESv2.dll
        ├── LICENSE
        ├── LICENSES.chromium.html
        ├── locales
         └── required Dependencies for independent performance
        ├── README.md
        ├── resources
         ├── app
          ├── .gitignore
          ├── .vscode
          ├── backend.c
          ├── backend.exe
          ├── bluedot.png
          ├── cart.txt
          ├── customer.c
          ├── customer.html
          ├── customer.js
          ├── delivery.c
          ├── delivery.html
          ├── delivery.js
          ├── deliveryman.txt
          ├── delivery_allocation.txt
          ├── dest.png
          ├── dijkstra.c
          ├── feedbacks.txt
          ├── icon.ico
          ├── icon.png
          ├── IPCcustomer.js
          ├── IPCdm.js
          ├── IPCloginReg.js
          ├── IPCrestaurant.js
          ├── location.txt
          ├── login.html
          ├── logo.png
          ├── main.js
          ├── map.png
          ├── mappin.webp
          ├── matrix.txt
          ├── menu.txt
          ├── nodemon.json
          ├── node_modules
            └── All the required node packages
          ├── order.txt
          ├── package.json
          ├── preload.js
          ├── README.md
          ├── recommend.c
          ├── register.html
          ├── register.js
          ├── renderer.js
          ├── restaurant.c
          ├── restaurant.html
          ├── restaurant.js
          ├── restaurants.txt
          ├── styles.css
          ├── transaction_final.txt
          └── users.txt
        ├── resources.pak
        ├── snapshot_blob.bin
        ├── tress.js
        ├── v8_context_snapshot.bin
        ├── version
        ├── vk_swiftshader.dll
        ├── vk_swiftshader_icd.json
        └── vulkan-1.dll

======================================= Backend Files Distribution ===============================================


How the Main Backend File has been Distributed Between several files for easy Code Management:

    backend.c
    ├── customer.c
    ├── delivery.c
    ├── dijkstra.c
    ├── recommend.c
    └── restaurant.c

    Database Files
    ├── cart.txt
    ├── deliveryman.txt
    ├── feedbacks.txt
    ├── location.txt
    ├── matrix.txt
    ├── menu.txt
    ├── order.txt
    ├── restaurants.txt
    ├── transaction_final.txt
    └── users.txt


======================================= FRONTEND AND BACKEND ===============================================

The frontend of the project has been contructed using Electron.js. Electron.js is a framework for building cross-platform desktop applications using web technologies like HTML, CSS, and JavaScript. It allows developers to create desktop apps with a single codebase that runs on Windows, macOS, and Linux.

Connecting an Electron frontend with a C backend using child processes involves leveraging Node.js's "child_process" module to spawn and manage the C program. When the Electron application needs to perform backend operations, it spawns the C executable as a child process using functions like "spawn" or "exec". The Electron frontend sends input data to the C backend through the child process's standard input (stdin). The C program processes the data and returns the results through standard output (stdout). The Electron application captures this output, processes it if necessary, and updates the user interface accordingly. This approach combines the high performance of C for backend operations with the rich, interactive capabilities of Electron for the frontend, enabling efficient and seamless data processing and user interaction. Proper handling of inputs, outputs, and errors is crucial for maintaining robust and efficient communication between the frontend and backend.

Example.

Sending Request From frontend:

    ipcRenderer.send("request", {function, data});

Fetching Request in electron Environment:

    ipcMain.on('request', (event, { function, dataset }) => {
    
    const backendProcess = spawn('./backend', [], { cwd: __dirname });          //Accessing C Backend and opening executable file.

    backendProcess.stdout.on('data', (data) => {                                //Fetching the output from terminal after giving input
        const result = JSON.parse(data.toString());
        global.mainWindow.webContents.send('response', { result });          //sending back the output to frontend from Electron Env
        console.log(result);
    });
    console.log(`${function} ${dataset}`);
    backendProcess.stdin.write(`${function} ${dataset}\n`);           //Writing on the terminal after execution to give input to backend
    });

On Terminal Example:
    let
    function = "getRestaurants";
    dataset = "testuser"

    getRestaurants testuser                 //Input into the executable terminal using child_process in Electron Env

    [                                               //Output from the Executable
       {
        username: 'FieMex',
        restaurantName: 'Fiesta Mexicana',
        contact: '012-345-6790',
        distance: '5',
        path: '55,69',
        rating: 2.94,
        address: '808 Fiesta Street, 01235'
        }
    ]

Processing the output response sent from the Electron environment:

    ipcRenderer.on("response", (event,result) => {
        console.log(result);
    });