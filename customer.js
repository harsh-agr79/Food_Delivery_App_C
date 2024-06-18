const { ipcRenderer } = window.electron;

if (ipcRenderer) {
  function plus_rec(id) {
    console.log(id);
    a = parseInt($(`#${id}recinp`).val());
    a = a + 1;
    $(`#${id}recinp`).val(a);
  }

  function minus_rec(id) {
    console.log(id);
    a = parseInt($(`#${id}recinp`).val());
    if (a != 0) {
      a = a - 1;
      $(`#${id}recinp`).val(a);
    }
  }

  function addRecToCart(id) {
    a = parseInt($(`#${id}recinp`).val());
    if (a == 0) {
      M.toast({ html: "Quantity Cannot Be Null" });
    } else {
      var func = "addRecToCart";
      var user = sessionStorage.getItem("username");
      var dataset = [user, id, a].join(",");
      ipcRenderer.send("addRecToCart", { func, dataset });
    }
  }

  ipcRenderer.on("addedRecToCart", (event, response) => {
    M.toast({ html: response.result });
    getCart();
  });

  document
    .getElementById("confirmCartBtn")
    .addEventListener("click", function () {
      var func = "confirmCart";
      var dataset = sessionStorage.getItem("username");
      ipcRenderer.send("confirmCart", { func, dataset });
    });

  ipcRenderer.on("confirmCartResponse", (event, response) => {
    res = response.result;
    var toastHTML = `<span>${res}</span>`;
    M.toast({ html: toastHTML });
    $("ul.tabs").tabs("select", "currentOrder");
  });

  getRestaurantipc();
  function getRestaurantipc() {
    var func = "getRestaurants";
    var dataset = sessionStorage.getItem("username");
    ipcRenderer.send("getRestaurants", { func, dataset });
  }
  document.getElementById("orderTab").addEventListener("click", function () {
    getRestaurantipc();
  });

  function initializeCanvas() {
    const img = document.getElementById("grid-image");
    const canvas = document.getElementById("drawing-canvas");

    // Set canvas size to match image size
    canvas.width = img.clientWidth;
    console.log(img.clientWidth);
    console.log(img.clientHeight);
    canvas.height = img.clientHeight;
    canvas.style.width = `${img.clientWidth}px`;
    canvas.style.height = `${img.clientHeight}px`;
    canvas.style.position = "absolute";
    canvas.style.top = `${img.offsetTop}px`;
    canvas.style.left = `${img.offsetLeft}px`;

    console.log("Canvas initialized");
  }

  function initializeCanvas2() {
    const img = document.getElementById("grid-image2");
    const canvas = document.getElementById("drawing-canvas2");

    // Set canvas size to match image size
    canvas.width = img.clientWidth;
    console.log(img.clientWidth);
    console.log(img.clientHeight);
    canvas.height = img.clientHeight;
    canvas.style.width = `${img.clientWidth}px`;
    canvas.style.height = `${img.clientHeight}px`;
    canvas.style.position = "absolute";
    canvas.style.top = `${img.offsetTop}px`;
    canvas.style.left = `${img.offsetLeft}px`;

    console.log("Canvas initialized");
  }

  function addPinToGrid(gridNumber, path, coordinates) {
    const rows = 12;
    const cols = 12;
    const totalCells = rows * cols;

    if (gridNumber < 0 || gridNumber >= totalCells) {
      console.error("Invalid grid number");
      return;
    }

    const img = document.getElementById("grid-image");
    const canvas = document.getElementById("drawing-canvas");

    // Set canvas size to match image size
    canvas.width = img.clientWidth;
    console.log(img.clientWidth);
    console.log(img.clientHeight);
    canvas.height = img.clientHeight;
    canvas.style.width = `${img.clientWidth}px`;
    canvas.style.height = `${img.clientHeight}px`;
    canvas.style.position = "absolute";
    canvas.style.top = `${img.offsetTop}px`;
    canvas.style.left = `${img.offsetLeft}px`;

    console.log("Canvas initialized");
    const container = img.parentElement;
    const rect = img.getBoundingClientRect();

    // Get image dimensions
    const imgWidth = img.clientWidth;
    const imgHeight = img.clientHeight;

    // Calculate the size of each grid cell
    const cellWidth = imgWidth / cols;
    const cellHeight = imgHeight / rows;

    // Calculate row and column of the grid number
    const row = Math.floor(gridNumber / cols);
    const col = gridNumber % cols;

    // Calculate center coordinates of the grid cell
    const centerX = col * cellWidth + cellWidth / 2;
    const centerY = row * cellHeight + cellHeight / 2;

    // Store coordinates for line drawing
    coordinates.push({ x: centerX + rect.left, y: centerY + rect.top });

    const pinIcon = document.createElement("img");

    if (gridNumber === path[0]) {
      pinIcon.src = "./mappin.webp";
      pinIcon.style.width = "12px";
    } else if (gridNumber === path[path.length - 1]) {
      pinIcon.src = "./dest.png";
      pinIcon.style.width = "20px";
    } else {
      pinIcon.style.width = "12px";
      pinIcon.src = "./bluedot.png";
    }
    pinIcon.style.position = "absolute";
    pinIcon.style.left = `${centerX + rect.left}px`;
    pinIcon.style.top = `${centerY + rect.top}px`;
    pinIcon.classList.add("pinco");

    container.appendChild(pinIcon);
  }

  function addPinToGrid2(gridNumber, path, coordinates) {
    const rows = 12;
    const cols = 12;
    const totalCells = rows * cols;

    if (gridNumber < 0 || gridNumber >= totalCells) {
      console.error("Invalid grid number");
      return;
    }

    const img = document.getElementById("grid-image2");
    const canvas = document.getElementById("drawing-canvas2");

    // Set canvas size to match image size
    canvas.width = img.clientWidth;
    console.log(img.clientWidth);
    console.log(img.clientHeight);
    canvas.height = img.clientHeight;
    canvas.style.width = `${img.clientWidth}px`;
    canvas.style.height = `${img.clientHeight}px`;
    canvas.style.position = "absolute";
    canvas.style.top = `${img.offsetTop}px`;
    canvas.style.left = `${img.offsetLeft}px`;

    console.log("Canvas initialized");
    const container = img.parentElement;
    const rect = img.getBoundingClientRect();

    // Get image dimensions
    const imgWidth = img.clientWidth;
    const imgHeight = img.clientHeight;

    // Calculate the size of each grid cell
    const cellWidth = imgWidth / cols;
    const cellHeight = imgHeight / rows;

    // Calculate row and column of the grid number
    const row = Math.floor(gridNumber / cols);
    const col = gridNumber % cols;

    // Calculate center coordinates of the grid cell
    const centerX = col * cellWidth + cellWidth / 2;
    const centerY = row * cellHeight + cellHeight / 2;

    // Store coordinates for line drawing
    coordinates.push({ x: centerX + rect.left, y: centerY + rect.top });

    const pinIcon = document.createElement("img");

    if (gridNumber === path[0]) {
      pinIcon.src = "./mappin.webp";
      pinIcon.style.width = "12px";
    } else if (gridNumber === path[path.length - 1]) {
      pinIcon.src = "./dest.png";
      pinIcon.style.width = "20px";
    } else {
      pinIcon.style.width = "12px";
      pinIcon.src = "./bluedot.png";
    }
    pinIcon.style.position = "absolute";
    pinIcon.style.left = `${centerX + rect.left}px`;
    pinIcon.style.top = `${centerY + rect.top}px`;
    pinIcon.classList.add("pinco");

    container.appendChild(pinIcon);
  }

  function drawLines(coordinates) {
    const canvas = document.getElementById("drawing-canvas");
    const ctx = canvas.getContext("2d");
    const rect = canvas.getBoundingClientRect();

    console.log("Drawing lines...");
    console.log("Canvas size:", canvas.width, canvas.height);

    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.beginPath();

    coordinates.forEach((coord, index) => {
      console.log("Coordinate:", coord);
      // Convert coordinates to canvas-relative
      const x = coord.x - rect.left + 5;
      const y = coord.y - rect.top + 5;

      console.log("Drawing line to:", x, y);
      if (index === 0) {
        ctx.moveTo(x, y);
      } else {
        ctx.lineTo(x, y);
      }
    });

    ctx.strokeStyle = "blue";
    ctx.lineWidth = 2;
    ctx.stroke();
  }

  function drawLines2(coordinates) {
    const canvas = document.getElementById("drawing-canvas2");
    const ctx = canvas.getContext("2d");
    const rect = canvas.getBoundingClientRect();

    console.log("Drawing lines...");
    console.log("Canvas size:", canvas.width, canvas.height);

    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.beginPath();

    coordinates.forEach((coord, index) => {
      console.log("Coordinate:", coord);
      // Convert coordinates to canvas-relative
      const x = coord.x - rect.left + 5;
      const y = coord.y - rect.top + 5;

      console.log("Drawing line to:", x, y);
      if (index === 0) {
        ctx.moveTo(x, y);
      } else {
        ctx.lineTo(x, y);
      }
    });

    ctx.strokeStyle = "blue";
    ctx.lineWidth = 2;
    ctx.stroke();
  }

  function pinPathMap(path) {
    const coordinates = [];
    path.forEach((item) => {
      addPinToGrid(item, path, coordinates);
    });
    console.log(coordinates);
    drawLines(coordinates);
  }

  function pinPathMap2(path) {
    const coordinates = [];
    path.forEach((item) => {
      addPinToGrid2(item, path, coordinates);
    });
    console.log(coordinates);
    drawLines2(coordinates);
  }
  document.getElementById("cartTab").addEventListener("click", function () {
    const img = document.getElementById("grid-image");
    const canvas = document.getElementById("drawing-canvas");

    // Set canvas size to match image size
    canvas.width = img.clientWidth;
    console.log(img.clientWidth);
    console.log(img.clientHeight);
    canvas.height = img.clientHeight;
    canvas.style.width = `${img.clientWidth}px`;
    canvas.style.height = `${img.clientHeight}px`;
    canvas.style.position = "absolute";
    canvas.style.top = `${img.offsetTop}px`;
    canvas.style.left = `${img.offsetLeft}px`;
    $("#resnameCart").text("");
    $("#resusernameCart").text("");
    $("#editCartBtn").removeAttr("onclick");
    $("#distanceCart").text("");
    $("#timetakenCart").text("");
    getCart();
    img.onload = function () {
      initializeCanvas();
    };
  });

  function recommendFood(id, user) {
    var func = "recommendFood";
    var dataset = [id, user].join(",");
    ipcRenderer.send("recommendFood", { func, dataset });
  }

  ipcRenderer.on("recommendFoodResponse", (event, response) => {
    res = response.result;

    const recommendBody = document.getElementById("cartRecommend");
    // console.log(res);
    recommendBody.innerHTML = "";
    res.forEach((item) => {
      const recItem = ` <div class="col s3 mp-card center" style="margin: 10px;">
            <h5 class="center">${item.food}</h5>
            <span class="center">${item.category} ${item.type}</span><br />
            <span center>${item.price}</span><br />
            <div class="row center" style="margin: 10px">
              <span class="col s3 prod-btn" onclick="minus_rec('${item.id}')" style="border-radius: 5px 0 0 5px"
                ><i class="material-icons">remove</i></span
              >
              <input
                type="number"
                min="0"
                id = "${item.id}recinp"
                value = "0"
                class="col s6 browser-default inp qtys orderItemQuantity"
                style="height: 32px; text-align: center; border-radius: 0"
              />
              <span class="col s3 prod-btn"  onclick="plus_rec('${item.id}')" style="border-radius: 0 5px 5px 0"
                ><i class="material-icons">add</i></span
              >
            </div>
            <div style="margin-bottom: 10px;">
              <button class="btn red darken-1" onclick="addRecToCart('${item.id}')">ADD Item</button>
            </div>
          </div>`;
      recommendBody.innerHTML += recItem;
    });
  });

  function getCart() {
    var func = "getCart";
    var dataset = sessionStorage.getItem("username");
    ipcRenderer.send("getCart", { func, dataset });
  }

  ipcRenderer.on("getCartResponse", (event, response) => {
    var res = response.result;
    console.log(res);
    const tableBody = document.getElementById("cartTableBody");
    var total = 0;

    tableBody.innerHTML = "";

    res.forEach((item) => {
      total += item.price * item.quantity;
      const row = `<tr id="${item.id}menutr">
      <td>${item.food}</td>
      <td>${item.category}</td>
      <td>${item.type}</td>
      <td>${item.quantity}</td>
      <td>${item.price}</td>
      <td>${item.price * item.quantity}<td>
      </tr>`;
      tableBody.innerHTML += row;
    });
    row = `<tr>
    <td></td>
    <td></td>
    <td></td>
    <td></td>
    <th>Total</th>
    <th>${total}<th>
    </tr>`;
    tableBody.innerHTML += row;
    var func = "getPathCart";
    var dataset = [res[0].customerUsername, res[0].restaurantUsername].join(
      ","
    );
    // console.log(dataset)
    $("#resnameCart").text(res[0].resname);
    $("#resusernameCart").text(res[0].restaurantUsername);
    $("#editCartBtn").attr(
      "onclick",
      `showMenu('${res[0].restaurantUsername}')`
    );
    ipcRenderer.send("getPathCart", { func, dataset });
    recommendFood(res[0].id, res[0].customerUsername);
  });

  document.getElementById("currentTab").addEventListener("click", function () {
    const img = document.getElementById("grid-image2");
    const canvas = document.getElementById("drawing-canvas2");

    // Set canvas size to match image size
    canvas.width = img.clientWidth;
    console.log(img.clientWidth);
    console.log(img.clientHeight);
    canvas.height = img.clientHeight;
    canvas.style.width = `${img.clientWidth}px`;
    canvas.style.height = `${img.clientHeight}px`;
    canvas.style.position = "absolute";
    canvas.style.top = `${img.offsetTop}px`;
    canvas.style.left = `${img.offsetLeft}px`;

    getCurrent();
    img.onload = function () {
      initializeCanvas2();
    };
  });

  function getCurrent() {
    $("#resnameCurrent").text("");
    $("#resusernameCurrent").text("");
    $("#statusCurrent").text("");
    $("#dmnameCurrent").text("");
    $("#distanceCurrent").text("");
    $("#timetakenCurrent").text("");
    $("#reviewCard").addClass("hide");
    $(".SRBTN").removeAttr("onclick");
    $("#starRatingValue").text("");
    $("#starRating .material-icons").removeClass("selected");
    $("#review").val("");
    var func = "getCurrent";
    var dataset = sessionStorage.getItem("username");
    ipcRenderer.send("getCurrent", { func, dataset });
  }

  ipcRenderer.on("getCurrentResponse", (event, response) => {
    var res = response.result;
    console.log(res);
    const tableBody = document.getElementById("currentTableBody");
    var total = 0;

    tableBody.innerHTML = "";

    res.forEach((item) => {
      total += item.price * item.quantity;
      const row = `<tr id="${item.id}menutr">
      <td>${item.food}</td>
      <td>${item.category}</td>
      <td>${item.type}</td>
      <td>${item.quantity}</td>
      <td>${item.price}</td>
      <td>${item.price * item.quantity}<td>
      </tr>`;
      tableBody.innerHTML += row;
    });
    row = `<tr>
    <td></td>
    <td></td>
    <td></td>
    <td></td>
    <th>Total</th>
    <th>${total}<th>
    </tr>`;
    tableBody.innerHTML += row;
    var func = "getPathCart";
    var dataset = [res[0].customerUsername, res[0].restaurantUsername].join(
      ","
    );
    // console.log(dataset)
    $("#resnameCurrent").text(res[0].resname);
    $("#resusernameCurrent").text(res[0].restaurantUsername);
    $("#statusCurrent").text(res[0].absolute_status);
    $("#dmnameCurrent").text(res[0].deliveryMan);
    if (res[0].review == "on") {
      $("#reviewCard").removeClass("hide");
      $(".SRBTN").attr(
        "onclick",
        `submitReview('${res[0].restaurantUsername}','${res[0].customerUsername}','${res[0].orderid}', this.value)`
      );
    } else {
      $("#reviewCard").addClass("hide");
      $(".SRBTN").removeAttr("onclick");
    }
    ipcRenderer.send("getPathCurrent", { func, dataset });
  });

  function searchRestaurant(search) {
    var func = "searchMenuForItemGiven";
    var dataset = [sessionStorage.getItem("username"), search].join(",");
    ipcRenderer.send("searchMenuForItemGiven", { func, dataset });
    if (search == null || search == "") {
      getRestaurantipc();
    }
  }

  ipcRenderer.on("searchRestaurantResponse", (event, response) => {
    var res = response.result;
    if (res.length < 1) {
      restList.innerHTML =
        "<h6 class='center-align red-text'>Can't Order Now, A Previous Order is Still Under Process</h6>";
    } else {
      const restList = document.getElementById("restList");
      restList.innerHTML = "";
      // res.sort((a,b) => parseInt(a.distance) - parseInt(b.distance));
      res.forEach((item) => {
        const restaurant = ` <div class="row mp-card restListItem" onclick="showMenu('${item.username}')">
            <div class="col s12"><h5>${item.restaurantName}</h5></div>
            <div class="col s12">${item.distance}000 Meters</div>
          <div class="col s12">Rating: ${item.rating} stars</div>
            <div class="col s12">${item.path}</div>
            <div class="col s12">${item.address}</div>
            <div class="col s12">${item.contact}</div>
          </div>`;
        restList.innerHTML += restaurant;
      });
    }
  });

  ipcRenderer.on("getPathCartResponse", (event, response) => {
    var res = response.result;
    var arr = res.split("|");
    var distance = arr[0];
    var path = arr[1].split(",");
    $("#distanceCart").text(distance);
    $("#timetakenCart").text(
      Math.round((parseInt(distance) * 1000) / 500 + 20)
    );
    // $("#timetakenCart").text(arr[1]);
    pinPathMap(path);
  });

  ipcRenderer.on("getPathCurrentResponse", (event, response) => {
    var res = response.result;
    var arr = res.split("|");
    var distance = arr[0];
    var path = arr[1].split(",");
    $("#distanceCurrent").text(distance);
    $("#timetakenCurrent").text(
      Math.round((parseInt(distance) * 1000) / 500 + 20)
    );
    // $("#timetakenCart").text(arr[1]);
    pinPathMap2(path);
  });

  ipcRenderer.on("getRestaurantResponse", (event, response) => {
    var res = response.result;
    if (res.length < 1) {
      restList.innerHTML =
        "<h6 class='center-align red-text'>Can't Order Now, A Previous Order is Still Under Process</h6>";
    } else {
      const restList = document.getElementById("restList");
      restList.innerHTML = "";
      // res.sort((a,b) => parseInt(a.distance) - parseInt(b.distance));
      res.forEach((item) => {
        const restaurant = ` <div class="row mp-card restListItem" onclick="showMenu('${item.username}')">
          <div class="col s12"><h5>${item.restaurantName}</h5></div>
          <div class="col s12">${item.distance}000 Meters</div>
          <div class="col s12">Rating: ${item.rating} stars</div>
          <div class="col s12">${item.path}</div>
          <div class="col s12">${item.address}</div>
          <div class="col s12">${item.contact}</div>
        </div>`;
        restList.innerHTML += restaurant;
      });
    }
  });

  function showMenu(username) {
    $("ul.tabs").tabs("select", "menuPage");
    var func = "getMenuCustomer";
    var dataset = [username, sessionStorage.getItem("username")].join(",");
    ipcRenderer.send("getMenuCustomer", { func, dataset });
    // $('#menuRestName').text(username);
    $("#menuContainer").text(username);
  }

  ipcRenderer.on("getMenuCustomerResponse", (event, response) => {
    var res = response.result;
    const menuBody = document.getElementById("menuContainer");
    // console.log(res);
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

  $(document).ready(function () {
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

      if (clickedId != "locationTab" || clickedId != "cartTab") {
        $("#pinicon").remove();
        $(".pinco").remove();
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
    var ids = $(".orderItemId")
      .map(function () {
        return this.value;
      })
      .get();
    var resname = $(".orderItemResname")
      .map(function () {
        return this.value;
      })
      .get();
    var username = $(".orderItemUsername")
      .map(function () {
        return this.value;
      })
      .get();
    var food = $(".orderItemFood")
      .map(function () {
        return this.value;
      })
      .get();
    var category = $(".orderItemCategory")
      .map(function () {
        return this.value;
      })
      .get();
    var type = $(".orderItemType")
      .map(function () {
        return this.value;
      })
      .get();
    var quantity = $(".orderItemQuantity")
      .map(function () {
        if (this.value > 0) {
          return this.value;
        } else {
          return "0";
        }
      })
      .get();
    var price = $(".orderItemPrice")
      .map(function () {
        return this.value;
      })
      .get();
    var count = ids.length;
    var data = [];

    for (i = 0; i < count; i++) {
      if (quantity[i] <= 0) {
        ids.splice(i, 1);
        resname.splice(i, 1);
        username.splice(i, 1);
        food.splice(i, 1);
        category.splice(i, 1);
        type.splice(i, 1);
        quantity.splice(i, 1);
        price.splice(i, 1);
        count--;
        i--;
      }
    }

    for (j = 0; j < count; j++) {
      data[j] = [
        custUN,
        resname[0],
        username[0],
        ids[j],
        food[j],
        category[j],
        type[j],
        quantity[j],
        price[j],
      ].join(",");
    }

    var dataset = [custUN, username[0], data.join("|")].join(",");
    // console.log(dataset);
    var func = "getUserCartInfo";
    ipcRenderer.send("getUserCartInfo", { func, dataset });
  }

  ipcRenderer.on("userCartInfoGet", (event, response) => {
    res = response.result;
    var custUN = sessionStorage.getItem("username");
    var ids = $(".orderItemId")
      .map(function () {
        return this.value;
      })
      .get();
    var resname = $(".orderItemResname")
      .map(function () {
        return this.value;
      })
      .get();
    var username = $(".orderItemUsername")
      .map(function () {
        return this.value;
      })
      .get();
    var food = $(".orderItemFood")
      .map(function () {
        return this.value;
      })
      .get();
    var category = $(".orderItemCategory")
      .map(function () {
        return this.value;
      })
      .get();
    var type = $(".orderItemType")
      .map(function () {
        return this.value;
      })
      .get();
    var quantity = $(".orderItemQuantity")
      .map(function () {
        if (this.value > 0) {
          return this.value;
        } else {
          return "0";
        }
      })
      .get();
    var price = $(".orderItemPrice")
      .map(function () {
        return this.value;
      })
      .get();
    var count = ids.length;
    var data = [];

    for (i = 0; i < count; i++) {
      if (quantity[i] <= 0) {
        ids.splice(i, 1);
        resname.splice(i, 1);
        username.splice(i, 1);
        food.splice(i, 1);
        category.splice(i, 1);
        type.splice(i, 1);
        quantity.splice(i, 1);
        price.splice(i, 1);
        count--;
        i--;
      }
    }

    for (j = 0; j < count; j++) {
      data[j] = [
        custUN,
        resname[0],
        username[0],
        ids[j],
        food[j],
        category[j],
        type[j],
        quantity[j],
        price[j],
      ].join(",");
    }

    var dataset = [custUN, username[0], data.join("|")].join(",");
    if (res == "No Change") {
      var func = "setUserCart";
      ipcRenderer.send("setUserCart", { func, dataset });
    } else {
      let userResponse = confirm(
        "You can order only from one restaurant. Do You want to clear the previous cart?"
      );

      if (userResponse) {
        var func = "setUserCart";
        ipcRenderer.send("setUserCart", { func, dataset });
      } else {
        $(".orderItemQuantity").val("0");
      }
    }
  });

  function goToCart() {
    $("ul.tabs").tabs("select", "cartPage");
  }

  ipcRenderer.on("userCartSet", (event, response) => {
    var res = response.result;
    var toastHTML = `<span>${res}</span><button class="btn-flat toast-action" onclick="goToCart()">Check Out</button>`;
    M.toast({ html: toastHTML });
  });

  document.getElementById("oldTab").addEventListener("click", () => {
    getOldOrder();
  });
  function getOldOrder() {
    var func = "getOldOrderCustomer";
    var dataset = sessionStorage.getItem("username");
    ipcRenderer.send("getOldOrderCustomer", { func, dataset });
  }

  ipcRenderer.on("oldOrderCustomerGet", (event, response) => {
    res = response.result;
    const tableBody = document.getElementById("oldOrderTbody");

    tableBody.innerHTML = "";

    res.forEach((item) => {
      const row = `<tr id="${item.orderid}menutr">
      <td>${item.time}</td>
      <td>${item.orderid}</td>
      <td>${item.restaurant_name}</td>
      <td><a onclick="viewBill(${item.orderid})" class="red darken-1 btn-small modal-trigger" href="#viewBillModal"><i class="material-icons">visibility</i></a><td>
      </tr>`;
      tableBody.innerHTML += row;
    });
  });

  function viewBill(orderid) {
    var func = "getViewBillCustomer";
    var dataset = orderid;
    ipcRenderer.send("getViewBillCustomer", { func, dataset });
  }

  ipcRenderer.on("viewBillCustomerGet", (event, response) => {
    res = response.result;
    const tableBody = document.getElementById("viewBillTbody");
    var total = 0;
    tableBody.innerHTML = "";
    $("#VBcustomerName").text(res[0].user_name);
    $("#VBrestaurantName").text(res[0].restaurantName);
    $("#VBstatus").text(res[0].absolute_status);

    res.forEach((item) => {
      total += item.price * item.quantity;
      const row = `<tr id="${item.itemid}menutr">
      <td>${item.itemname}</td>
      <td>${item.item_category}</td>
      <td>${item.item_type}</td>
      <td>${item.quantity}</td>
      <td>${item.price}</td>
      <td>${item.price * item.quantity}</td>
      </tr>`;
      tableBody.innerHTML += row;
    });
    row = `<tr>
    <td></td>
    <td></td>
    <td></td>
    <td></td>
    <th>Total</th>
    <th>${total}<th>
    </tr>`;
    tableBody.innerHTML += row;
  });

  $(document).ready(function () {
    $(".star-rating .material-icons").on("click", function () {
      var ratingValue = $(this).data("value");
      $("#starRating .material-icons").removeClass("selected");
      $(this).prevAll().addBack().addClass("selected");
      $("#starRatingValue").text(ratingValue);
    });
  });

  function submitReview(rest, cust, oid, type) {
    if (type == "later") {
      $("#starRatingValue").text("");
      $("#review").val("");
    }
    var rating = $("#starRatingValue").text();
    var review = $("#review").val();
    var func = "submitReview";
    if (rating == "") {
      rating = "NULL";
    }
    if (review == "") {
      review = "NULL";
    }
    var dataset = [oid, cust, rest, rating, review].join(",");
    if (type == "submit" && rating == "NULL" && review == "NULL") {
      $("#starRatingValue").text(
        "Rating and Comment Required to Submit Review."
      );
    } else {
      ipcRenderer.send("submitReview", { func, dataset });
    }
  }

  ipcRenderer.on("reviewSubmitted", (event, response) => {
    res = response.result;
    M.toast({ html: res });
    getCurrent();
  });

  document.getElementById("profileTab").addEventListener("click", () => {
    getCustomerProfile();
  });

  function getCustomerProfile(){
    var func = "getCustomerProfile";
    var dataset = sessionStorage.getItem('username');
    ipcRenderer.send("getCustomerProfile", {func,dataset});
  }

  ipcRenderer.on("customerProfileGet", (event, response) => {
    res = response.result;
    $('#rpname').val(res.name);
    $('#rpusername').val(res.username);
    $('#rppassword').val(res.password);
    $('#rpcontact').val(res.contact);
    $('#rpaddress').val(res.address);
    $('#rppincode').val(res.pincode);
  })

  const profileForm = document.getElementById("customerProfileForm");

  profileForm.addEventListener('submit', (e) => {
    e.preventDefault();
    var inp =  $('.rp').map(function() {
        return this.value;
    }).get();
    const func = "editCustomerProfile";
    const dataset = inp.join(",");
    ipcRenderer.send('editCustomerProfile', { func, dataset });
})

ipcRenderer.on("customerProfileEdited", (event,response) => {
  res = response.result;
  M.toast({ html: res });
})

  function logout() {
    ipcRenderer.send("logout");
  }
  ipcRenderer.on("clearSession", (event) => {
    sessionStorage.clear();
    console.log("clear");
    M.toast({ html: "Logged Out" });
    ipcRenderer.send("gotologin");
  });
  $('input').bind('input', function() {
    var c = this.selectionStart,
        r = /[^a-z0-9 .-]/gi,
        v = $(this).val();
    if(r.test(v)) {
      $(this).val(v.replace(r, ''));
      c--;
    }
    this.setSelectionRange(c, c);
  });
} else {
  console.error("ipcRenderer is not properly initialized.");
}
