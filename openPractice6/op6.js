const https = require("https")
const url = `https://www.skku.edu/skku/campus/skk_comm/notice01.do`
const header = {
    headers: {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/106.0.0.0 Safari/537.36"
    }
}
https.get {
    url, header, (res) => {
        let data = ""
        
    }
}

console.log("complete")