const zigbeeHerdsmanConverters = require('zigbee-herdsman-converters');
const zigbeeHerdsmanUtils = require('zigbee-herdsman-converters/lib/utils');


const exposes = zigbeeHerdsmanConverters['exposes'] || require("zigbee-herdsman-converters/lib/exposes");
const ea = exposes.access;
const e = exposes.presets;
const modernExposes = (e.hasOwnProperty('illuminance_lux'))? false: true;

const fz = zigbeeHerdsmanConverters.fromZigbeeConverters || zigbeeHerdsmanConverters.fromZigbee;
const tz = zigbeeHerdsmanConverters.toZigbeeConverters || zigbeeHerdsmanConverters.toZigbee;

const localFromZigbee = [fz.occupancy, fz.on_off];
// for old versions of Z2M
if (fz.ignore_basic_report) {
  localFromZigbee.unshift(fz.ignore_basic_report);
}

const device = {
    zigbeeModel: ['foxnest.presence-detector'],
    model: 'foxnest.presence-detector',
    vendor: 'Fox-Nest Inc',
    description: 'DIY presence detector',
    fromZigbee: localFromZigbee,
    toZigbee: [tz.on_off],
    exposes: [
        e.occupancy(),
        // Target 1
        e.binary("occupancy", ea.STATE, true, false)
            .withEndpoint("target_1_valid")
            .withDescription("Target 1 is valid"),
        e.numeric("x", ea.STATE)
            .withEndpoint("target_1_x")
            .withUnit("mm")
            .withValueMin(-10000)
            .withValueMax(10000)
            .withDescription("Target 1 X coordinate (millimeters)"),
        e.numeric("y", ea.STATE)
            .withEndpoint("target_1_y")
            .withUnit("mm")
            .withValueMin(0)
            .withValueMax(10000)
            .withDescription("Target 1 Y coordinate (millimeters)"),
        // Target 2
        e.binary("occupancy", ea.STATE, true, false)
            .withEndpoint("target_2_valid")
            .withDescription("Target 2 is valid"),
        e.numeric("x", ea.STATE)
            .withEndpoint("target_2_x")
            .withUnit("mm")
            .withValueMin(-10000)
            .withValueMax(10000)
            .withDescription("Target 2 X coordinate (millimeters)"),
        e.numeric("y", ea.STATE)
            .withEndpoint("target_2_y")
            .withUnit("mm")
            .withValueMin(0)
            .withValueMax(10000)
            .withDescription("Target 2 Y coordinate (millimeters)"),
        // Target 3
        e.binary("occupancy", ea.STATE, true, false)
            .withEndpoint("target_3_valid")
            .withDescription("Target 3 is valid"),
        e.numeric("x", ea.STATE)
            .withEndpoint("target_3_x")
            .withUnit("mm")
            .withValueMin(-10000)
            .withValueMax(10000)
            .withDescription("Target 3 X coordinate (millimeters)"),
        e.numeric("y", ea.STATE)
            .withEndpoint("target_3_y")
            .withUnit("mm")
            .withValueMin(0)
            .withValueMax(10000)
            .withDescription("Target 3 Y coordinate (millimeters)"),
    ],
    meta: {
        multiEndpoint: true,
        binaryEndpoints: {'l2': 'contact', 'Beep': 'contact', 'l4': 'contact', }, 
    },
    endpoint: (device) => {
        return {
            occupancy: 1,
            target_1_valid: 2,
            target_1_x: 3,
            target_1_y: 4,
            target_2_valid: 5,
            target_2_x: 6,
            target_2_y: 7,
            target_3_valid: 8,
            target_3_x: 9,
            target_3_y: 10,
        };
    },
    configure: async (device, coordinatorEndpoint, logger) => {
      let saveDevice = false;
      const endpoint = device.getEndpoint(1);
      await endpoint.read('genBasic', ['modelId', 'swBuildId', 'powerSource']);
      if (saveDevice) { device.save(); }
    },

};

module.exports = device;
