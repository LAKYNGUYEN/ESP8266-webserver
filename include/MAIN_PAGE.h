#ifndef MAIN_PAGE_H
#define MAIN_PAGE_H


const char *MAIN_PAGE = R"html(
<!doctypehtml><html lang=en><meta charset=UTF-8><meta content="width=device-width,initial-scale=1"name=viewport><title>ESP8266</title><link href=https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css rel=stylesheet crossorigin=anonymous integrity=sha384-QWTKZyjpPEjISv5WaRU9OFeRpok6YctnYmDr5pNlyT2bRjXh0JMhjY6hW+ALEwIH><link href=https://cdn.jsdelivr.net/gh/hung1001/font-awesome-pro@4cac1a6/css/all.css rel=stylesheet><style>.main-content{margin-top:50px;width:100%;max-width:450px}.form-check-control{width:50px!important;height:25px!important}.card{border-width:0!important;box-shadow:0 0 10px rgba(0,0,0,.1);border-radius:12px;margin-bottom:40px}.card-circle{display:flex;justify-content:center;align-items:center;width:150px;height:150px;border-radius:200px}</style><div class="container-fluid main-content"><div class=card><div class="d-flex align-items-center card-body"><div><span class=fs-4>LED Builtin</span></div><div class="form-check form-switch ms-auto"><input class="form-check-control form-check-input"id=led-btn role=switch type=checkbox></div></div></div><div class="container mt-5"><div class="d-flex justify-content-center"><div class="card-circle bg-danger-subtle"><span class=fs-3><i class="far fa-thermometer-half"></i> <span class=temp-text></span></span></div><div class="card-circle bg-info-subtle ms-5"><span class=fs-3><i class="far fa-humidity"></i> <span class=humi-text></span></span></div></div></div></div><script crossorigin=anonymous integrity=sha384-YvpcrYf0tY3lHB60NNkmXc5s9fDVZLESaAA55NDzOxhy9GkcIdslK1eN7N6jIeHz src=https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/js/bootstrap.bundle.min.js></script><script>const ws = new WebSocket(`ws://${window.location.host}:81`)
const $btn = document.querySelector('#led-btn')

// HTTP GET request
function req(ep, cb) {
    fetch(ep)
        .then((res) => res.text())
        .then((data) => {
            if (cb)
                cb(data)
        })
        .catch((err) => {
            console.log(err)
        })
}

function updateButton(state) {
    $btn.checked = state === '0' ? true : false
}

// Kiem tra thai hien tai cua led
req('/getstatus', updateButton)

// Khi ma nhan nut

$btn.addEventListener('click', (e) => {
    req('/toggle', updateButton)
})</script><script>ws.addEventListener('message', (e) => {
    const msg = e.data
    if (msg.startsWith('LED_STATE')) {
        updateButton(msg.split(':')[1])
    }

    if (msg.startsWith('READ_DHT')) {
        const [temp, humi] = msg.split(':')[1].split('-')
        document.querySelector('.temp-text').innerHTML = temp + '°C'
        document.querySelector('.humi-text').innerText = humi + '%'
    }
})</script>
)html";


#endif // MAIN_PAGE_H