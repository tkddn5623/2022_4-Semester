function multiplyNumeric(obj) {
    for(let x in obj) {
        if(typeof(obj[x]) === "number") obj[x] *= 2
    }
}
let menu = {
    width: 200,
    height: 300,
    title: "My menu"
}

multiplyNumeric(menu)
for(let x in menu) {
    console.log(menu[x])
}
console.log(typeof("Ss"))