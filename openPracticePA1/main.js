const Med = (arr) => {
    let num, medIdx = parseInt(arr.length / 2)
    if (arr.length % 2 == 0) num = (arr[medIdx - 1] + arr[medIdx]) / 2
    else num = arr[medIdx]
    return num
}
const Quartile = (arr) => {
    if(arr.length == 1) return arr[0]
    let arr1 = [], arr2 = []
    const mid = parseInt(arr.length / 2)
    arr1 = arr.slice(0, mid);
    if (arr.length % 2 == 0) arr2 = arr.slice(mid, arr.length)
    else arr2 = arr.slice(mid + 1, arr.length)
    return [Med(arr1), Med(arr2)]
}
const Iqr = (arr) => {
    let ret = Quartile(arr)
    return ret[1] - ret[0]
}
const Outlier = (arr, arr_origin) => {
    let qt = Quartile(arr)
    let iqr = qt[1] - qt[0]
    return arr_origin.filter(it => {
        if (it < qt[0] - 1.5 * iqr || it > qt[1] + 1.5 * iqr) return true
        else return false
    })
}

const command = process.argv[2]
let arr = []
for (let i = 2; i < process.argv.length; i++) {
    if (!isNaN(Number(process.argv[i]))) arr.push(Number(process.argv[i]))
}
const arr_const = arr.map((it) => { return it })
arr.sort((a, b) => { return a - b })
switch (command) {
    case 'med':
        console.log(Med(arr))
        break
    case 'iqr':
        console.log(Iqr(arr))
        break
    case 'outlier':
        Outlier(arr, arr_const).forEach(it => { console.log(it) })
        break
    default:
        console.log('Please enter correct command')
        break
}
