let websock;
let isAuth = false;
let isLogin = false;
let cred_list = [];

col = [
    { name: 'id', title: 'id' },
    { name: 'website', title: 'website' },
    { name: 'username', title: 'username' },
    { name: 'email', title: 'email' },
    { name: 'password', title: 'password' },
]

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
            email: '*****@***.com',
            username: '*******',
            password: '**********',
        }
    });

}
function show_cred_table() {
    let value="";
    cred_list.map(dir => {
        value += `<tr data-id='${dir.id}'>`
        value += `<th scope="row">${dir.id}</th>`
        value += `<td> ${dir.website} </td>`
        value += `<td> ${dir.email} </td>`
        value += `<td> ${dir.username} </td>`
        value += `<td id="pass${dir.id}"> ********** </td>`
        value += `</tr>`

    });
    document.getElementById('main_table').innerHTML = value;
    $('.table > tbody > tr').click(function () {
        console.log("clicked", $(this).data("id"));
        edit($(this).data("id"));
      });
}
function socketMessageListener(evt) {
    let data = JSON.parse(evt.data)
    switch (data.resultof) {
        case "auth":
            if (data.state = "success") {
                console.log("Aithed Successfully!");
                isAuth = true;
                hide("#signin");
                start("#login");
            }
            break;
        case "login":
            if (data.state = "success") {
                console.log("Login Successfully!");
                isLogin = true;
                hide("#login");
                console.log("Listing");
                list();
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


function edit(id) {
    document.querySelector('#email').value = cred_list[id].email;
    document.querySelector('#username').value = cred_list[id].username;
    document.querySelector('#website').value = cred_list[id].website;
    document.querySelector('#password').value = cred_list[id].password;
    start("#edit");
}

function start(x) {
    esprfidcontent = document.createElement("div");
    esprfidcontent.id = "mastercontent";
    esprfidcontent.style.display = "none";
    document.body.appendChild(esprfidcontent);
    $(x).on("shown.bs.modal", function () {
        $("#password").focus().select();
    });
    $("#mastercontent").load("esprfid.htm", function (responseTxt, statusTxt, xhr) {
        if (statusTxt === "success") {
            $(x).modal({ backdrop: "static", keyboard: false });
            $("[data-toggle=\"popover\"]").popover({
                container: "body"
            });

        }
    });
}

function hide(id) {
    $(id).modal("hide");
}

