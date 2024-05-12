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
    inp.unshift(sessionStorage.getItem("username"))
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
      const row = document.createElement("tr");

      // Populate table cells with data from the JSON object
      const cells = [
        item.food,
        item.category,
        item.price.toFixed(2), // Format price to 2 decimal places
      ];

      // Append each cell to the table row
      cells.forEach((cellData) => {
        const cell = document.createElement("td");
        cell.textContent = cellData;
        row.appendChild(cell);
      });

      // Append the row to the table body
      tableBody.appendChild(row);
    });
  });

  ipcRenderer.on("getAddItemResponse", (event, response) => {
    var res = response.result;
    getmenuipc();
    M.toast({html: 'Item Added To Menu!'});
    $('#addMenuItem').modal('close');
    $(".addItemInp").val('');
  });
} else {
  console.error("ipcRenderer is not properly initialized.");
}
