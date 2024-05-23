const { ipcRenderer } = window.electron;

if (ipcRenderer) {
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
} else {
  console.error("ipcRenderer is not properly initialized.");
}
