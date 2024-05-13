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

  function delMenuFunc(id){
    dataset = id
    func = "deleteMenuItem";
    ipcRenderer.send("deleteMenuItem", { func, dataset });
  }

  function editMenuFunc(id){
    var datatr = document.getElementById(`${id}menutr`);
    var tds = Array.from(datatr.querySelectorAll('td'));

    $("#editFoodId").val(id)
    $("#editFoodName").val(tds[0].textContent)
    $("#editFoodCategory").val(tds[1].textContent)
    $("#editFoodPrice").val(tds[2].textContent)
  }
} else {
  console.error("ipcRenderer is not properly initialized.");
}
