const { ipcRenderer } = window.electron;

if (ipcRenderer) {

  document.getElementById("newAllocTab"),addEventListener("click", (e) => {
    getNewAlloc();
  })

  

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
    console.log("hello")
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
    var func = "setDeliveryManLocation";
    ipcRenderer.send("setDeliveryManLocation", { func, dataset });

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

  ipcRenderer.on("deliveryManLocationSet", (event, response) => {
    var res = response.result;
    console.log(res);
  });
  ipcRenderer.on("deliveryManLocationGet", (event, response) => {
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
    func = "getDeliveryManLocation";
    dataset = sessionStorage.getItem("username");
    ipcRenderer.send("getDeliveryManLocation", { func, dataset });
  });

  $(document).ready(function () {
    func = "getDeliveryManLocation";
    dataset = sessionStorage.getItem("username");
    ipcRenderer.send("getDeliveryManLocation", { func, dataset });
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

  function pinPathMap(path) {
    const coordinates = [];
    path.forEach((item) => {
      addPinToGrid(item, path, coordinates);
    });
    console.log(coordinates);
    drawLines(coordinates);
  }

  function drawLines(coordinates) {
    const canvas = document.getElementById('drawing-modal-canvas');
    const ctx = canvas.getContext('2d');
    const rect = canvas.getBoundingClientRect();

    console.log("Drawing lines...");
    console.log("Canvas size:", canvas.width, canvas.height);

    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.beginPath();

    coordinates.forEach((coord, index) => {
        console.log("Coordinate:", coord);
        // Convert coordinates to canvas-relative
        const x = (coord.x - rect.left - 15);
        const y = (coord.y - rect.top + 10);

        console.log("Drawing line to:", x, y);
        if (index === 0) {
            ctx.moveTo(x, y);
        } else {
            ctx.lineTo(x, y);
        }
    });

    ctx.strokeStyle = 'blue';
    ctx.lineWidth = 2;
    ctx.stroke();
}

  function addPinToGrid(gridNumber, path, coordinates) {
    const rows = 12;
    const cols = 12;
    const totalCells = rows * cols;

    if (gridNumber < 0 || gridNumber >= totalCells) {
      console.error("Invalid grid number");
      return;
    }

    const img = document.getElementById("grid-modal-image");
    const canvas = document.getElementById("drawing-modal-canvas");

    // Set canvas size to match image size
    canvas.width = img.clientWidth;
    console.log(img.clientWidth);
    console.log(img.clientHeight);
    canvas.height = img.clientHeight;
    canvas.style.width = `${img.clientWidth}px`;
    canvas.style.height = `${img.clientHeight}px`;
    canvas.style.position = 'absolute';
    canvas.style.top = `${img.offsetTop}px`;
    canvas.style.left = `${img.offsetLeft}px`;

    console.log("Canvas initialized");
    const container = img.parentElement;
    const rect = img.getBoundingClientRect();
    // const rect2 = canvas.getBoundingClientRect();

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
    pinIcon.style.left = `${centerX + img.offsetLeft - 20}px`;
    pinIcon.style.top = `${centerY  + img.offsetTop  + 3}px`;
    pinIcon.classList.add("pinco");

    container.appendChild(pinIcon);
  }

  function viewPath(path){
    $("#pinicon").remove();
    $(".pinco").remove();
    var newpath = path.split(",");
    setTimeout(() => {
      pinPathMap(newpath)
    }, 1000);
    // pinPathMap(newpath);
    // console.log(newpath);
  }

  function acceptDelivery(orderid){
    var dataset = [orderid,sessionStorage.getItem("username")].join(",");
    var func = "acceptDelivery"
    ipcRenderer.send("acceptDelivery", {func, dataset});
  }

  ipcRenderer.on("acceptedDelivery", (event, response) => {
    var res = response.result;
    M.toast({ html: res });
    getNewAlloc();
  })

 

  function getNewAlloc(){
    var func = "getNewAllocation";
    var dataset = sessionStorage.getItem("username");
    ipcRenderer.send("getNewAllocation", { func, dataset });
  }

  ipcRenderer.on("newAllocationGet", (event, response) => {
    res = response.result;
    const tableBody = document.getElementById("newAllocTbody");

    tableBody.innerHTML = "";

    res.forEach((item) => {
      const row = `<tr id="${item.orderid}menutr">
      <td>${item.orderid}</td>
      <td>${item.customerName}</td>
      <td>${item.restaurantName}</td>
      <td>${item.distance}000 meters</td>
      <td><a onclick="viewPath('${item.path}')" class="amber btn-small modal-trigger" href="#viewMapModal"><i class="material-icons">visibility</i></a><td>
      <td><a onclick="acceptDelivery(${item.orderid})" class="amber btn-small">Accept</a><td>
      </tr>`;
      tableBody.innerHTML += row;
    });
  })


  
} else {
  console.error("ipcRenderer is not properly initialized.");
}
