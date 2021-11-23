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
    cred_list = cred.map(dir => {
        return {
            website: dir,
            email: null,
            username: null,
            password: null,
        }
    });

}
function socketMessageListener(evt) {
    let data = JSON.parse(evt.data)
    switch (data.resultof) {
        case "auth":
            if (data.state = "success") {
                console.log("Aithed Successfully!");
                isAuth = true;
                toggleModal();
                // window.location.href = "login.html";
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