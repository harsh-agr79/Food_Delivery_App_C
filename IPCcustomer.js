const { ipcMain } = require('electron');
const { spawn } = require('child_process');

ipcMain.on('getRestaurants', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = JSON.parse(data.toString());
        global.mainWindow.webContents.send('getRestaurantResponse', { result });
        console.log(result);
    });
    console.log(`${func} ${dataset}`);
    
    backendProcess.stdin.write(`${func} ${dataset}\n`);
});

ipcMain.on('searchMenuForItemGiven', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = JSON.parse(data.toString());
        global.mainWindow.webContents.send('searchRestaurantResponse', { result });
        console.log(result);
    });
    console.log(`${func} ${dataset}`);
    
    backendProcess.stdin.write(`${func} ${dataset}\n`);
});

ipcMain.on('setCustomerLocation', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = parseInt(data.toString());
        global.mainWindow.webContents.send('customerLocationSet', { result });
        console.log(result);
    });
    console.log(`${func} ${dataset}`);

    backendProcess.stdin.write(`${func} ${dataset}\n`);
});

ipcMain.on('getCustomerLocation', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = data.toString();
        global.mainWindow.webContents.send('customerLocationGet', { result });
        console.log(result);
    });
    console.log(`${func} ${dataset}`);
    backendProcess.stdin.write(`${func} ${dataset}\n`);
});

ipcMain.on('setUserCart', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = data.toString();
        global.mainWindow.webContents.send('userCartSet', { result });
        console.log(result);
    });
    console.log(`${func} ${dataset}`);
    backendProcess.stdin.write(`${func} ${dataset}\n`);
});

ipcMain.on('getMenuCustomer', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = JSON.parse(data.toString());
        global.mainWindow.webContents.send('getMenuCustomerResponse', { result });
        console.log(result);
    });
    console.log(`${func} ${dataset}`);

    backendProcess.stdin.write(`${func} ${dataset}\n`);
});

ipcMain.on('getCart', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = JSON.parse(data.toString());
        global.mainWindow.webContents.send('getCartResponse', { result });
        console.log(result);
    });
    console.log(`${func} ${dataset}`);

    backendProcess.stdin.write(`${func} ${dataset}\n`);
});
ipcMain.on('getPathCart', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = data.toString();
        global.mainWindow.webContents.send('getPathCartResponse', { result });
        console.log(result);
    });
    console.log(`${func} ${dataset}`);

    backendProcess.stdin.write(`${func} ${dataset}\n`);
});

ipcMain.on('confirmCart', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = data.toString();
        global.mainWindow.webContents.send('confirmCartResponse', { result });
        console.log(result);
    });
    console.log(`${func} ${dataset}`);

    backendProcess.stdin.write(`${func} ${dataset}\n`);
});


ipcMain.on('getCurrent', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = JSON.parse(data.toString());
        global.mainWindow.webContents.send('getCurrentResponse', { result });
        console.log(result);
    });
    console.log(`${func} ${dataset}`);

    backendProcess.stdin.write(`${func} ${dataset}\n`);
});
ipcMain.on('getPathCurrent', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = data.toString();
        global.mainWindow.webContents.send('getPathCurrentResponse', { result });
        console.log(result);
    });
    console.log(`${func} ${dataset}`);

    backendProcess.stdin.write(`${func} ${dataset}\n`);
});
ipcMain.on('getOldOrderCustomer', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = JSON.parse(data.toString());
        global.mainWindow.webContents.send('oldOrderCustomerGet', { result });
        console.log(result);
    });
    console.log(`${func} ${dataset}`);
    backendProcess.stdin.write(`${func} ${dataset}\n`);
});
ipcMain.on('getViewBillCustomer', (event, { func, dataset }) => {
    const backendProcess = spawn('./backend', [], { cwd: __dirname });

    backendProcess.stdout.on('data', (data) => {
        const result = JSON.parse(data.toString());
        global.mainWindow.webContents.send('viewBillCustomerGet', { result });
        console.log(result);
    });
    console.log(`${func} ${dataset}`);
    backendProcess.stdin.write(`${func} ${dataset}\n`);
});