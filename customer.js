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
     <div class="col s6">${item.price}</div>
     <div class="col s6">Add to Cart</div>
   </div>`
   menuBody.innerHTML += menuItem;
    });
  });
} else {
  console.error("ipcRenderer is not properly initialized.");
}
