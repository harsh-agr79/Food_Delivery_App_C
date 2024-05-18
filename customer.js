const { ipcRenderer } = window.electron;

if (ipcRenderer) {
  getRestaurantipc();
  function getRestaurantipc() {
    var func = "getRestaurants";
    var dataset = "NULL";
    ipcRenderer.send("getRestaurants", { func, dataset });
  }
  document.getElementById("orderTab").addEventListener("click", function () {
    getRestaurantipc();
  });
  ipcRenderer.on("getRestaurantResponse", (event, response) => {
    var res = response.result;
    const restList = document.getElementById("restList");
    restList.innerHTML = "";
    res.forEach((item) => {
      const restaurant = ` <div class="row mp-card restListItem" onclick="showMenu('${item.username}')">
          <div class="col s12"><h5>${item.restaurantName}</h5></div>
          <div class="col s12">${item.address}</div>
          <div class="col s12">${item.contact}</div>
        </div>`;
      restList.innerHTML += restaurant;
    });
  });

  function showMenu(username){
    $('ul.tabs').tabs("select", "menuPage");
    var func = "getMenu";
    var dataset = username;
    ipcRenderer.send("getMenu", { func, dataset });
    $('#menuRestName').text(username);
    $('#menuContainer').text(username);
  }

  ipcRenderer.on("getMenuResponse", (event, response) => {
    var res = response.result;
    const menuBody = document.getElementById("menuContainer");
    console.log(res);
    menuBody.innerHTML = "";

    res.forEach((item) => {
     const menuItem = ` <div class="row mp-card">
     <div class="col s6">${item.food}</div>
     <div class="col s6">${item.category}</div>
     <br>
     <br>
     <div class="col s6">${item.price}</div>
     <div class="col s6"><div class="row container">
     <span class="col s3 prod-btn" style="border-radius: 5px 0 0 5px;"
         onclick="minus('${item.id}')"><i class="material-icons">remove</i></span>
     <input type="hidden" class="prodids" name="prodid[]">
     <input type="number" min="0" class="col s6 browser-default inp qtys" value = "0" id="${item.id}cartinp"
         style="height: 32px; text-align:center; border-radius:0;"
         name="qty[]">
     <span class="col s3 prod-btn" style="border-radius: 0 5px 5px 0; "
         onclick="plus('${item.id}')"><i class="material-icons">add</i></span>
 </div></div>
   </div>`
   menuBody.innerHTML += menuItem;
    });
  });

  const mapdom = document.getElementById("map");

  const gridnum = [];
  const gridsize = 12;

  let count = 0;
  for (let i = 0; i < gridsize; i++) {
    gridnum[i] = [];
    for (let j = 0; j < gridsize; j++) {
      gridnum[i][j] = count++;
    }
  }
  document.getElementById("map").addEventListener("click", function (e) {
    $("#pinicon").remove();
    const imgwidth = mapdom.width;
    const imgheight = mapdom.height;
    const cellwidth = imgwidth / gridsize;
    const cellheight = imgheight / gridsize;

    const rect = mapdom.getBoundingClientRect();
    const x = e.clientX - rect.left;
    const y = e.clientY - rect.top;

    const gridX = Math.floor(x / cellwidth);
    const gridY = Math.floor(y / cellheight);

    const num = gridnum[gridY][gridX];
    const username = sessionStorage.getItem("username");
    const usertype = sessionStorage.getItem("usertype");
    var dataset = [username,usertype ,num, e.clientX, e.clientY].join(",");
    var func = "setCustomerLocation";
    ipcRenderer.send("setCustomerLocation", { func, dataset });

    const pinIcon = document.createElement("img");
    pinIcon.src = "./mappin.webp"; // Replace 'location-pin.png' with your pin icon image URL
    pinIcon.style.position = "absolute";
    pinIcon.style.height = "20px";
    pinIcon.style.left = `${e.clientX}px`;
    pinIcon.style.top = `${e.clientY}px`;
    pinIcon.id = "pinicon";
    pinIcon.style.transform = "translate(-50%, -100%)"; // Adjust position for centering pin icon

    // Append the pin icon element to the document body or a container element
    document.body.appendChild(pinIcon);
  });

  ipcRenderer.on("customerLocationSet", (event, response) => {
    var res = response.result;
    console.log(res);
  });
  ipcRenderer.on("customerLocationGet", (event, response) => {
    $("#pinicon").remove();
    var res = response.result;
    // console.log(res);
    var data = res.split(",");
    const pinIcon = document.createElement("img");
    pinIcon.src = "./mappin.webp"; // Replace 'location-pin.png' with your pin icon image URL
    pinIcon.style.position = "absolute";
    pinIcon.style.height = "20px";
    pinIcon.style.left = `${data[3]}px`;
    pinIcon.style.top = `${data[4]}px`;
    pinIcon.id = "pinicon";
    pinIcon.style.transform = "translate(-50%, -100%)";

    document.body.appendChild(pinIcon);

  });
  document.getElementById("locationTab").addEventListener("click", () => {
    func = "getCustomerLocation";
    dataset = sessionStorage.getItem("username");
    ipcRenderer.send("getCustomerLocation", { func, dataset });
  });

  const elements = document.querySelectorAll(".tab");

  // Attach a click event listener to each element
  elements.forEach((element) => {
    element.addEventListener("click", (event) => {
      // Access the id of the clicked element
      const clickedId = event.target.id;

      if(clickedId != "locationTab"){
        $("#pinicon").remove();
      }
    });
  });


  function plus(id) {
    a = parseInt($(`#${id}cartinp`).val())
    a = a + 1
    $(`#${id}cartinp`).val(a)
    // updatecart()
}

function minus(id) {
    a = parseInt($(`#${id}cartinp`).val())
    if (a != 0) {
        a = a - 1
        $(`#${id}cartinp`).val(a)
    }
    // updatecart()
}
} else {
  console.error("ipcRenderer is not properly initialized.");
}
