const { ipcRenderer } = window.electron;

if (ipcRenderer) {
  getRestaurantipc();
  function getRestaurantipc() {
    var func = "getRestaurants";
    var dataset = sessionStorage.getItem("username");
    ipcRenderer.send("getRestaurants", { func, dataset });
  }
  document.getElementById("orderTab").addEventListener("click", function () {
    getRestaurantipc();
  });
  ipcRenderer.on("getRestaurantResponse", (event, response) => {
    var res = response.result;
    const restList = document.getElementById("restList");
    restList.innerHTML = "";
    // res.sort((a,b) => parseInt(a.distance) - parseInt(b.distance));
    res.forEach((item) => {
      const restaurant = ` <div class="row mp-card restListItem" onclick="showMenu('${item.username}')">
          <div class="col s12"><h5>${item.restaurantName}</h5></div>
          <div class="col s12">${item.distance}00 Meters</div>
          <div class="col s12">${item.path}</div>
          <div class="col s12">${item.address}</div>
          <div class="col s12">${item.contact}</div>
        </div>`;
      restList.innerHTML += restaurant;
    });
  });

  function showMenu(username) {
    $("ul.tabs").tabs("select", "menuPage");
    var func = "getMenuCustomer";
    var dataset = [username,sessionStorage.getItem("username")].join(",");
    ipcRenderer.send("getMenuCustomer", { func, dataset });
    // $('#menuRestName').text(username);
    $("#menuContainer").text(username);
  }

  ipcRenderer.on("getMenuCustomerResponse", (event, response) => {
    var res = response.result;
    const menuBody = document.getElementById("menuContainer");
    console.log(res);
    menuBody.innerHTML = "";
    $("#menuRestName").text(res[0].restaurantName);
    res.forEach((item) => {
      const menuItem = ` <div class="row mp-card">
     <input type="hidden" class="orderItemId" value="${item.id}" />
     <input type="hidden" class="orderItemResname" value="${item.restaurantName}" />
     <input type="hidden" class="orderItemUsername" value="${item.username}" />
     <input type="hidden" class="orderItemFood" value="${item.food}" />
     <input type="hidden" class="orderItemCategory" value="${item.category}" />
     <input type="hidden" class="orderItemType" value="${item.type}" />
     <input type="hidden" class="orderItemPrice" value="${item.price}" />
     <div class="col s4">${item.food}</div>
     <div class="col s4">${item.category}</div>
     <div class="col s4">${item.type}</div>
     <br>
     <br>
     <div class="col s6">${item.price}</div>
     <div class="col s6"><div class="row container">
     <span class="col s3 prod-btn" style="border-radius: 5px 0 0 5px;"
         onclick="minus('${item.id}')"><i class="material-icons">remove</i></span>
     <input type="hidden" class="prodids" name="prodid[]">
     <input type="number" min="0" class="col s6 browser-default inp qtys orderItemQuantity" value = "${item.quantity}" id="${item.id}cartinp"
         onchange="updatecart();" onkeyup="updatecart();" style="height: 32px; text-align:center; border-radius:0;"
         name="qty[]">
     <span class="col s3 prod-btn" style="border-radius: 0 5px 5px 0; "
         onclick="plus('${item.id}')"><i class="material-icons">add</i></span>
 </div></div>
   </div>`;
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
    var dataset = [username, usertype, num, e.clientX, e.clientY].join(",");
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

      if (clickedId != "locationTab") {
        $("#pinicon").remove();
      }
    });
  });

  function plus(id) {
    a = parseInt($(`#${id}cartinp`).val());
    a = a + 1;
    $(`#${id}cartinp`).val(a);
    updatecart();
  }

  function minus(id) {
    a = parseInt($(`#${id}cartinp`).val());
    if (a != 0) {
      a = a - 1;
      $(`#${id}cartinp`).val(a);
    }
    updatecart();
  }

  function updatecart() {
    var custUN = sessionStorage.getItem("username");
    var ids = $(".orderItemId").map(function () {return this.value;}).get()
    var resname = $(".orderItemResname").map(function () {return this.value;}).get()
    var username = $(".orderItemUsername").map(function () {return this.value;}).get()
    var food = $(".orderItemFood").map(function () {return this.value;}).get()
    var category = $(".orderItemCategory").map(function () {return this.value;}).get()
    var type = $(".orderItemType").map(function () {return this.value;}).get()
    var quantity = $(".orderItemQuantity").map(function () {if(this.value > 0) {return this.value }else{return "0"}}).get()
    var price = $(".orderItemPrice").map(function () {return this.value;}).get()
    var count = ids.length;
    var data = [];

    for(i = 0; i<count;i++){
      if(quantity[i] <= 0){
        ids.splice(i,1);
        resname.splice(i,1);
        username.splice(i,1);
        food.splice(i,1);
        category.splice(i,1);
        type.splice(i,1);
        quantity.splice(i,1);
        price.splice(i,1);
        count--;
        i--;
      }
    }

    for(j = 0; j<count; j++){
      data[j] = [custUN, resname[0], username[0], ids[j], food[j], category[j], type[j], quantity[j], price[j]].join(",");
    }

    var dataset = [custUN,username[0],data.join("|")].join(",");
    console.log(dataset);
    var func = "setUserCart";
    ipcRenderer.send("setUserCart", { func, dataset });
  }

  ipcRenderer.on("userCartSet", (event, response) => {
    var res = response.result;
    var toastHTML = `<span>${res}</span><button class="btn-flat toast-action">Check Out</button>`;
    M.toast({html: toastHTML});
  });


} else {
  console.error("ipcRenderer is not properly initialized.");
}
