const { ipcRenderer } = window.electron;

if (ipcRenderer) {
  $(document).ready(function () {
    getmenuipc();
  });

  document.getElementById("menuTab").addEventListener("click", () => {
    getmenuipc();
  });

  document.getElementById("recentTab").addEventListener("click", () => {
    getRecentOrder();
  });
  document.getElementById("currentTab").addEventListener("click", () => {
    getCurrentOrder();
  });
  document.getElementById("oldTab").addEventListener("click", () => {
    getOldOrder();
  });
  function viewBill(orderid){
    var func = "getViewBill";
    var dataset = orderid;
    ipcRenderer.send("getViewBill", {func,dataset}); 
  }

  function getRecentOrder(){
    var func = "getRecentOrder";
    var dataset = sessionStorage.getItem("username");
    ipcRenderer.send("getRecentOrder", { func, dataset });
  }

  ipcRenderer.on('recentOrderGet', (event, response) => {
    res = response.result;
    const tableBody = document.getElementById("recentOrderTbody");

    tableBody.innerHTML = "";

    res.forEach((item) => {
      const row = `<tr id="${item.orderid}menutr">
      <td>${item.time}</td>
      <td>${item.orderid}</td>
      <td>${item.user_name}</td>
      <td>${item.contact}</td>
      <td>${item.address}, ${item.pincode}</td>
      <td><a onclick="viewBill(${item.orderid})" class="amber btn-small modal-trigger" href="#viewBillModal"><i class="material-icons">visibility</i></a><td>
      </tr>`;
      tableBody.innerHTML += row;
    });
  })
  function getCurrentOrder(){
    var func = "getCurrentOrder";
    var dataset = sessionStorage.getItem("username");
    ipcRenderer.send("getCurrentOrder", { func, dataset });
  }

  ipcRenderer.on('currentOrderGet', (event, response) => {
    res = response.result;
    const tableBody = document.getElementById("currentOrderTbody");

    tableBody.innerHTML = "";

    res.forEach((item) => {
      const row = `<tr id="${item.orderid}menutr">
      <td>${item.time}</td>
      <td>${item.orderid}</td>
      <td>${item.user_name}</td>
      <td>${item.contact}</td>
      <td>${item.address}, ${item.pincode}</td>
      <td><a onclick="viewBill(${item.orderid})" class="amber btn-small modal-trigger" href="#viewBillModal"><i class="material-icons">visibility</i></a><td>
      </tr>`;
      tableBody.innerHTML += row;
    });
  })
  function getOldOrder(){
    var func = "getOldOrder";
    var dataset = sessionStorage.getItem("username");
    ipcRenderer.send("getOldOrder", { func, dataset });
  }

  ipcRenderer.on('oldOrderGet', (event, response) => {
    res = response.result;
    const tableBody = document.getElementById("oldOrderTbody");

    tableBody.innerHTML = "";

    res.forEach((item) => {
      const row = `<tr id="${item.orderid}menutr">
      <td>${item.time}</td>
      <td>${item.orderid}</td>
      <td>${item.user_name}</td>
      <td>${item.contact}</td>
      <td>${item.address}, ${item.pincode}</td>
      <td><a onclick="viewBill(${item.orderid})" class="amber btn-small modal-trigger" href="#viewBillModal"><i class="material-icons">visibility</i></a><td>
      </tr>`;
      tableBody.innerHTML += row;
    });
  })

  function changeOrderStatus(dataset){
    var func = "changeOrderStatus"
    ipcRenderer.send("changeOrderStatus", {func, dataset});
  }

  ipcRenderer.on("orderStatusChange", (event, response) => {
    var res = response.result;
    M.toast({ html: res });
    getRecentOrder();
    $("#viewBillModal").modal("close");
  } )

 

  ipcRenderer.on("viewBillGet", (event, response) => {
    res = response.result;
    const tableBody = document.getElementById("viewBillTbody");
    var total = 0;
    tableBody.innerHTML = "";
    $('#VBcustomerName').text(res[0].user_name);
    $('#VBcustomerContact').text(res[0].contact);
    $('#VBcustomerAddress').text(`${res[0].address}, ${res[0].pincode}`);
    $('#VBrejectBTN').attr("onclick", `changeOrderStatus("${res[0].orderid},reject")`);
    $('#VBacceptBTN').attr("onclick", `changeOrderStatus("${res[0].orderid},accept")`);
    $('#VBstatus').text(res[0].absolute_status);

    if(res[0].status === "accept"){
      $("#VBmodalfooter").addClass("hide");
      // console.log("VBMF");
    }
    else{
      $("#VBmodalfooter").removeClass("hide");
      // console.log("VBMFNOT");
    }
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
  })

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
      <td>${item.type}</td>
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

  ipcRenderer.on("getFormResponse", (event, response) => {
    var res = response.result;
    console.log(res);
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
    console.log(tds);
    $("#editFoodId").val(id);
    $("#editFoodName").val(tds[0].textContent);
    $("#editFoodCategory").val(tds[1].textContent);
    $("#editFoodType").val(tds[2].textContent);
    $("#editFoodPrice").val(tds[3].textContent);
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
    const usertype = sessionStorage.getItem("usertype");
    func = "setRestaurantLocation";
    var dataset = [username,usertype ,num, e.clientX, e.clientY].join(",");
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
  function logout(){
    ipcRenderer.send("logout");
  }
  ipcRenderer.on("clearSession", (event) => {
    sessionStorage.clear();
    console.log("clear");
    ipcRenderer.send("gotologin");
  });
} else {
  console.error("ipcRenderer is not properly initialized.");
}
