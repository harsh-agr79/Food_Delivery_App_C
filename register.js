const { ipcRenderer } = window.electron;

if (ipcRenderer) {

    const registerForm = document.getElementById('registerForm')
    const message = document.getElementById('message');


registerForm.addEventListener('submit', (e) => {
    e.preventDefault();
    var inp =  $('.inp').map(function() {
        return this.value;
    }).get();
    const tryfunc = String(inp[0]);
    inp.shift();
    const dataset = inp.join(",");
    ipcRenderer.send('registersubmit', { tryfunc, dataset });
})

ipcRenderer.on('registerResponse', (event, response) => {
    // console.log(response);
    if (response.result == 1) {
        $('#message').html("Registeration Successful ! Go To Login ");
        var inp =  $('.inp').val('');
    } 
    else if(response.result == 500){
        $('#message').html("Username Already Taken");
        $("#username").val('');
        $('#username').focus();
    }
    else {
        message.textContent = "Registration failed. Please try again.";
        console.log("err");
    }
});

document.getElementById('gotologin').addEventListener('click', () => {
    ipcRenderer.send('gotologin');
});
} else {
    console.error('ipcRenderer is not properly initialized.');
}