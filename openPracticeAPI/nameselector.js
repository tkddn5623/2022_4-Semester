function getfoodinfo(fooddb, searchword) {
    const searchresult = fooddb['records'].filter(food => food.식품명 === searchword)
    const range = ['식품명']
    let output = []
    range.forEach(info => output[info] = searchresult[0][info])
    return output
}
const fooddb = require('./fooddb.json')
let namelist = []

fooddb['records'].forEach((item) => {
    for (key in item) {
        if(key === '식품명') namelist.push(item[key])
    }
})
console.log(namelist)
const fs = require('fs')
fs.writeFileSync('ttt.json', JSON.stringify(namelist))