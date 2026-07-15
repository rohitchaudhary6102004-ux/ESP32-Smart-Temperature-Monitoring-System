
let temperature = 32;
let humidity = 60;

const temp = document.getElementById("temp");
const hum = document.getElementById("hum");
const time = document.getElementById("time");

function updateTime() {
    const now = new Date();

    time.innerHTML = now.toLocaleTimeString();
}

async function updateSensor() {
    try {
        const tempResponse = await fetch("/temperature");
        const humResponse = await fetch("/humidity");

        const temperature = await tempResponse.text();
        const humidity = await humResponse.text();

        temp.innerHTML = temperature + "°C";

        hum.innerHTML = humidity + "%";

        updateTime();
    }
    catch (error) {
        console.log("Fetch Error:", error);
    }

}

updateSensor();

setInterval(updateSensor, 1000);