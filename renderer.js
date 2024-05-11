// renderer.js
const { ipcRenderer } = window.electron;
const { sessionStorage } = window;

if (ipcRenderer) {
  const loginForm = document.getElementById("loginForm");
  const message = document.getElementById("message");

  loginForm.addEventListener("submit", (e) => {
    e.preventDefault();
    const username = document.getElementById("username").value;
    const password = document.getElementById("password").value;
    var data = [username, password];
    var dataset = data.join(",");
    var tryfunc = "login";
    ipcRenderer.send("login", { tryfunc, dataset });
  });

  ipcRenderer.on("loginResponse", (event, response) => {
    var res = response.result.split(",");
    console.log(res);
    res[res.length - 1] = res[res.length - 1].slice(0, -2);
    if (res[0] == 1) {
      message.textContent = "Login successful!";
      var inp =  $('.inp').val('');
      sessionStorage.setItem('login', true);
      sessionStorage.setItem('username', res[1]);
      sessionStorage.setItem('usertype', res[2]);
      ipcRenderer.send('mainMenu', { res });
    } 
    else {
      message.textContent = "Login failed. Please try again.";
      var inp =  $('.inp').val('');
      sessionStorage.clear();
    }
  });

  document.getElementById("register").addEventListener("click", () => {
    ipcRenderer.send("register");
  });
} else {
  console.error("ipcRenderer is not properly initialized.");
}
