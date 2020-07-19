/**
 * Get palette from https://github.com/kgolid/chromotome/tree/master/palettes
 * and save them as json file with this node script
 *
 * You need to change the the export statement of each palette file
 * by changing "export default" to "module.exports = {
 * palette: function () {
 * 		return ["
 */

const fs = require('fs');
const path = require('path');
const system = require('./js/system');
const rohlfs = require('./js/rohlfs');
const misc = require('./js/misc');
const judson = require('./js/judson');

const paletteGroup = [];
paletteGroup['system'] = system.palette();
paletteGroup['rohlfs'] = rohlfs.palette();
paletteGroup['misc'] = misc.palette();
paletteGroup['judson'] = judson.palette();

for (let palette in paletteGroup) {
	const jsonFile = './json/' + palette + '.json';
	fs.writeFile(jsonFile, JSON.stringify(paletteGroup[palette]), function (err) {
		if (err) return console.log(err);
		console.log(palette + ' palette converted (' + jsonFile + ')');
	});
}
