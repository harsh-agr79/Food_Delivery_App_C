const { ipcRenderer } = window.electron;

if (ipcRenderer) {
  $(document).ready(function () {
    getmenuipc();
  });

  document.getElementById("menuTab").addEventListener("click", () => {
    console.log("works");
    getmenuipc();
  });



  const addMenuItemForm = document.getElementById("addMenuItemForm");

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
      const row = `<tr>
      <td>${item.food}</td>
      <td>${item.category}</td>
      <td>${item.price}</td>
      <td><span onclick="delMenuFunc(${item.id})" class="red btn-small delMenuBtn"><i class="material-icons">delete</i></span><td>
      </tr>`;
      tableBody.innerHTML += row ;
    });
  });

  ipcRenderer.on("getAddItemResponse", (event, response) => {
    var res = response.result;
    getmenuipc();
    M.toast({ html: "Item Added To Menu!" });
    $("#addMenuItem").modal("close");
    $(".addItemInp").val("");
  });

  ipcRenderer.on("getDeleteItemResponse", (event, response) => {
    var res = response.result;
    getmenuipc();
    M.toast({ html: "Item Deleted!" });
  });

  function delMenuFunc(id){
    dataset = id
    func = "deleteMenuItem";
    ipcRenderer.send("deleteMenuItem", { func, dataset });
  }
} else {
  console.error("ipcRenderer is not properly initialized.");
}
