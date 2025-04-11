// Even I don't want to look at this code.
const inputField = document.getElementById("inputField");
const outputField = document.getElementById("outputField");
const submitBtn = document.getElementById("inputForm");
const encoder = new TextEncoder();
const decoder = new TextDecoder();
const sumField = document.getElementById("sum");
const negative = document.getElementById("negative");
const zero = document.getElementById("zero");
const carry = document.getElementById("carry");

let writer = null;
let reader = null;
let port = null;
let response = "";
let input = "";

let negFlag = false;
let zeroFlag = true;
let carryFlag = false;
let sum = 0;

negative.checked = negFlag;
zero.checked = zeroFlag;
carry.checked = carryFlag;
sumField.placeholder = parseInt(response[0]);


outputField.innerHTML = `command: ${response[0]}<br><span class="highlight">→</span>output: ${response[1]}`

submitBtn.addEventListener("submit", getResponse);
window.addEventListener('click', selectPort);

// Main
async function getResponse(event) {
  event.preventDefault();

  input = inputField.value;
  await writer.write(encoder.encode(input));
  const { value, done } = await reader.read();
  if (!value || done) {
    return;
  }
  response = decoder.decode(value);
  const a = response.split("|");

  console.log(a[1]);
  if (a[1]) {
    sum = toDec(a[1].slice(1, 3).padStart(8, "0"));
    negFlag = a[1][3] == "1";
    zeroFlag = a[1][4] == "1";
    carryFlag = a[1][5] == "1";
    sumField.value = sum;

    negative.checked = negFlag;
    zero.checked = zeroFlag;
    carry.checked = carryFlag;
  }
  outputField.innerHTML = `command: ${a[0].substring(0, 13)}<br><span class="highlight">→</span>output: ${a[1]}`
}

async function selectPort() {
  if (!port) {
    try {
      port = await navigator.serial.requestPort();
      await port.open({baudRate: 9600});
      writer = port.writable.getWriter();
      reader = port.readable.getReader();
    }
    catch (error) {
      console.error(error);
    }
  }
}

function getBit(number, index) {
  return number >> index & 1;
}

function toDec(str = "00000000") {
  let s = 0;
  for (let i = 0; i < 8; i++) {
    s += Math.pow(2, i) * parseInt(str[7-i]);
  }
  return s;
}