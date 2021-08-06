let websock;
function connectWS() {
    let wsUri = `ws://${document.getElementById('ip_input').value}/ws`;
    console.log(wsUri);
    websock = new WebSocket(wsUri);
    websock.addEventListener("message", socketMessageListener);
    websock.onopen = function (evt) {
        console.log("Websocket Opened Successfully.");
        console.log(evt);

    };
}
function sendMessage() {
    let payload = document.getElementById("textbox").value;
    if (!!websock) {
        websock.send(payload);
    }

}
function socketMessageListener(evt) {
    console.log(evt.data);
}