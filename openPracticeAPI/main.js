function getfoodinfo(fooddb, searchword) {
    const searchresult = fooddb['records'].filter(food => food.식품명 === searchword)
    if (searchresult.length == 0) throw new Error("음식을 찾지 못했습니다")
    const range = ['식품명', '식품중량', '에너지(kcal)', '탄수화물(g)', '단백질(g)', '지방(g)']
    let output = []
    range.forEach(info => output[info] = searchresult[0][info])
    return output
}
const fooddb = require('./fooddb.json')


let searchword1 = "삼겹살구이"
let foodinfo1 = getfoodinfo(fooddb, searchword1)
let searchword2 = "조개구이_키조개"
let foodinfo2 = getfoodinfo(fooddb, searchword2)
console.log(foodinfo1)
console.log(foodinfo2)