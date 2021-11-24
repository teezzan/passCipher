let websock;
let isAuth = false;
let isLogin = false;
let cred_list = [];
function connectWS() {
    let wsUri = `ws://${document.getElementById('ip').value}/ws`;
    console.log(wsUri);
    websock = new WebSocket(wsUri);
    websock.addEventListener("message", socketMessageListener);
    websock.onopen = function (evt) {
        console.log("Websocket Opened Successfully.");
        authorize(document.getElementById('pin').value);

    };
}

function authorize(pass) {
    let data = {
        command: "auth",
        pass
    }
    websock.send(JSON.stringify(data));
}

function login(pass) {
    let data = {
        command: "login",
        pass: "ovuvog"
    }
    websock.send(JSON.stringify(data));
}

function list() {
    let data = {
        command: "list"
    }
    websock.send(JSON.stringify(data));
}
function delete_cred(dir) {
    let data = {
        command: "delete_credential",
        dir
    }
    websock.send(JSON.stringify(data));
}

function get_credential(dir) {
    let data = {
        command: "get_credential",
        dir
    }
    websock.send(JSON.stringify(data));
}

function reload() {
    let data = {
        command: "reload"
    }
    websock.send(JSON.stringify(data));
}

function add_credential(email, username, password, website) {
    let data = {
        command: "add_credential",
        email,
        username,
        password,
        website
    }
    websock.send(JSON.stringify(data));
}
function add_credential_without_password(email, username, website) {
    let data = {
        command: "add_credential_gen_password",
        email,
        username,
        website
    }
    websock.send(JSON.stringify(data));
}

function populate_credentials(cred) {
    let id = 0;
    cred_list = cred.map(dir => {
        id++;
        return {
            id,
            website: dir,
            email: null,
            username: null,
            password: null,
        }
    });

}
function show_cred_table() {
    let value;
    cred_list.map(dir => {
        value += `<div class='table-row-1'><h4> ${dir.id} </h4>`
        value += `<h4> ${dir.website} </h4>`
        value += `<h4> ${dir.username} </h4>`
        value += `<h4 id="pass${dir.id}"> ********** </h4>`
        value += `<span onclick="myFunction(${dir.id})"> <img alt='/' src='option.svg' id='option'> </span></div>`

    });
    document.getElementById('main_table').innerHTML = value;
}
function socketMessageListener(evt) {
    let data = JSON.parse(evt.data)
    switch (data.resultof) {
        case "auth":
            if (data.state = "success") {
                console.log("Aithed Successfully!");
                isAuth = true;
                toggleModal();
            }
            break;
        case "login":
            if (data.state = "success") {
                console.log("Login Successfully!");
                isLogin = true;
                list();
                document.getElementById("mod").style.display = 'none';
            }
            break;
        case "list":
            if (data.state = "success") {
                console.log("List Successfully!");
                populate_credentials(data.credentials)
                console.log(cred_list);
                show_cred_table()
            }
            break;
        case "add_credential":
            if (data.state = "success") {
                console.log("Added Successfully!");
                reload();
                list();
            }
            break;
        case "delete_credential":
            if (data.state = "success") {
                console.log("Removed Successfully!");
                reload();
                list();
            }
            break;
        case "get_credential":
            if (data.state = "success") {
                console.log("Added Successfully!");
                let index = cred_list.findIndex((credential) => credential.website == data.dir)
                console.log(index);
                if (index != -1) {
                    cred_list[index].email = data.email
                    cred_list[index].password = data.password
                    cred_list[index].username = data.username
                }
                console.log(cred_list[index])
            }
            break;
        default:
            console.log(evt);
            break;
    }
}


function show(shown, hidden) {
    document.getElementById(shown).style.display = 'block';
    document.getElementById(hidden).style.display = 'none';
}
function toggleModal() {
    var x = document.getElementById("conntab");
    var y = document.getElementById("passtab");
    if (x.style.display === "none") {
        x.style.display = "block";
        y.style.display = "none";
    } else {
        x.style.display = "none";
        y.style.display = "block";
    }
}
function myFunction(id){
    console.log(id);
}