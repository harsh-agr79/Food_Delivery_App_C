const { ipcRenderer } = window.electron;

if (ipcRenderer) {
  $(document).ready(function () {
    getmenuipc();
  });

  document.getElementById("menuTab").addEventListener("click", () => {
    getmenuipc();
  });

  const addMenuItemForm = document.getElementById("addMenuItemForm");
  const editMenuItemForm = document.getElementById("editMenuItemForm");

  addMenuItemForm.addEventListener("submit", (e) => {
    e.preventDefault();
    var inp = $(".addItemInp")
      .map(function () {
        return this.value;
      })
      .get();
    inp.unshift(sessionStorage.getItem("username"));
    const dataset = inp.join(",");
    func = "addMenuItem";
    ipcRenderer.send("addMenuItem", { func, dataset });
  });
  editMenuItemForm.addEventListener("submit", (e) => {
    e.preventDefault();
    var inp = $(".editItemInp")
      .map(function () {
        return this.value;
      })
      .get();
    // inp.unshift(sessionStorage.getItem("username"));
    const dataset = inp.join(",");
    func = "editMenuItem";
    ipcRenderer.send("editMenuItem", { func, dataset });
  });

  function getmenuipc() {
    var func = "getMenu";
    var dataset = sessionStorage.getItem("username");
    ipcRenderer.send("getMenu", { func, dataset });
  }

  ipcRenderer.on("getMenuResponse", (event, response) => {
    var res = response.result;
    const tableBody = document.getElementById("menuTableBody");

    tableBody.innerHTML = "";

    res.forEach((item) => {
      const row = `<tr id="${item.id}menutr">
      <td>${item.food}</td>
      <td>${item.category}</td>
      <td>${item.price}</td>
      <td><a onclick="editMenuFunc(${item.id})" class="amber btn-small modal-trigger" href="#editMenuItem"><i class="material-icons">edit</i></a><td>
      <td><span onclick="delMenuFunc(${item.id})" class="red btn-small delMenuBtn"><i class="material-icons">delete</i></span><td>
      </tr>`;
      tableBody.innerHTML += row;
    });
  });

  ipcRenderer.on("getAddItemResponse", (event, response) => {
    var res = response.result;
    getmenuipc();
    M.toast({ html: "Item Added To Menu!" });
    $("#addMenuItem").modal("close");
    $(".addItemInp").val("");
  });

  ipcRenderer.on("getEditItemResponse", (event, response) => {
    var res = response.result;
    getmenuipc();
    M.toast({ html: "Item Edited To Menu!" });
    $("#editMenuItem").modal("close");
    $(".editItemInp").val("");
  });

  ipcRenderer.on("getDeleteItemResponse", (event, response) => {
    var res = response.result;
    getmenuipc();
    M.toast({ html: "Item Deleted!" });
  });

  function delMenuFunc(id) {
    dataset = id;
    func = "deleteMenuItem";
    ipcRenderer.send("deleteMenuItem", { func, dataset });
  }

  function editMenuFunc(id) {
    var datatr = document.getElementById(`${id}menutr`);
    var tds = Array.from(datatr.querySelectorAll("td"));

    $("#editFoodId").val(id);
    $("#editFoodName").val(tds[0].textContent);
    $("#editFoodCategory").val(tds[1].textContent);
    $("#editFoodPrice").val(tds[2].textContent);
  }

  //MAP FUNCTIONS
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
    func = "setRestaurantLocation";
    var dataset = [username, num, e.clientX, e.clientY].join(",");
    func = "setRestaurantLocation";
    ipcRenderer.send("setRestaurantLocation", { func, dataset });

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

  ipcRenderer.on("restaurantLocationSet", (event, response) => {
    var res = response.result;
    console.log(res);
  });
  ipcRenderer.on("restaurantLocationGet", (event, response) => {
    $("#pinicon").remove();
    var res = response.result;
    var data = res.split(",");
    const pinIcon = document.createElement("img");
    pinIcon.src = "./mappin.webp"; // Replace 'location-pin.png' with your pin icon image URL
    pinIcon.style.position = "absolute";
    pinIcon.style.height = "20px";
    pinIcon.style.left = `${data[2]}px`;
    pinIcon.style.top = `${data[3]}px`;
    pinIcon.id = "pinicon";
    pinIcon.style.transform = "translate(-50%, -100%)";

    document.body.appendChild(pinIcon);

  });
  document.getElementById("settingsTab").addEventListener("click", () => {
    func = "getRestaurantLocation";
    dataset = sessionStorage.getItem("username");
    ipcRenderer.send("getRestaurantLocation", { func, dataset });
  });

  //Define Click event for map pin
  const elements = document.querySelectorAll(".tab");

  // Attach a click event listener to each element
  elements.forEach((element) => {
    element.addEventListener("click", (event) => {
      // Access the id of the clicked element
      const clickedId = event.target.id;

      if(clickedId != "settingsTab"){
        $("#pinicon").remove();
      }
    });
  });
} else {
  console.error("ipcRenderer is not properly initialized.");
}
