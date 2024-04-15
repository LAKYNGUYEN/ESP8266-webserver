#ifndef MAIN_PAGE_H
#define MAIN_PAGE_H


const char *MAIN_PAGE = R"html(
<!doctypehtml><html lang=en><meta charset=UTF-8><meta content="IE=edge"http-equiv=X-UA-Compatible><meta content="width=device-width,initial-scale=1"name=viewport><title>LED controller</title><style>html{font-family:Arial,Helvetica,sans-serif;margin:0 auto;text-align:center}a{text-decoration:none;color:#ede6e6}.container{background-color:#3b757b;width:300px;height:auto;border-radius:10px;margin:auto;padding:10px}body{background-color:#efefef;margin-top:50px}h1{color:#ede7e7;margin:50px 30px}h3{color:#f3f3f3;margin-bottom:50px}.container.button{display:block;width:100px;text-align:center;border:none;color:#d7e6e4;padding:50px 20px;text-decoration:none;font-size:25px;margin:0 auto 35px;cursor:pointer;border-radius:10px}.container .button{background-color:rgba(255 255 255 / .3);padding:5px 10px;color:#fff;border-radius:5px}.container .button:active{background-color:rgba(255 255 255 / .5)}.container .button-on{background-color:#00f}p{font-size:15px;color:#98c29d;margin:5px}</style><div class=container><h1>ESP8266 Wedsever</h1><h3>Control Led</h3><p>Led Status</p><button class=button>ON</button></div><div class=container style=margin-top:40px><h3>Nhiệt độ: <span class=temp-text></span></h3><h3>Độ ẩm: <span class=humi-text></span></h3></div><script>const ws = new WebSocket(`ws://${window.location.host}:81`)
    const $btn = document.querySelector('.button')

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
        state === '0'
            ? $btn.classList.add('button-on')
            : $btn.classList.remove('button-on') 
        $btn.innerHTML = state === '0' ? 'ON' : 'OFF'
    }

    // Kiem tra thai hien tai cua led
    req('/getstatus', updateButton)

    // Khi ma nhan nut

    $btn.addEventListener('click', (e) => {
        req('/toggle', updateButton)
    })</script><script>ws.addEventListener('message', (e) => {
        const msg = e.data
        console.log(msg)

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