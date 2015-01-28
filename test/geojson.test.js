var osmium = require('../');
var assert = require('assert');

describe('geojson', function() {

   it('should be able to create geojson from a node', function(done) {
        var file = new osmium.File(__dirname + "/data/winthrop.osm");
        var reader = new osmium.BasicReader(file, {node: true});
        var stream = new osmium.Stream(reader);

        var count = 0;
        stream.set_callback('node', function(node) {
            if (count++ == 0) {
                assert.deepEqual(node.geojson(), {
                    type: 'Point',
                    coordinates: [-120.1891610, 48.4655800]
                });
                done();
            }
        });

        stream.on('data', stream.dispatch);
    });

   it('should be able to create geojson from a way', function(done) {
        var file = new osmium.File(__dirname + "/data/winthrop.osm");
        var location_handler = new osmium.LocationHandler();
        var reader = new osmium.Reader(file, location_handler, { node: true, way: true });
        var filter = new osmium.Filter();
        filter.with_ways();
        var stream = new osmium.Stream(reader, filter);

        stream.set_callback('way', function(way) {
            if (way.id == 6089456) {
                assert.deepEqual(way.geojson(), {
                    type: 'LineString',
                    coordinates: [
                        [-120.1796227, 48.4798110],
                        [-120.1787663, 48.4802976]
                    ]
                });
                done();
            }
        });

        stream.on('data', stream.dispatch);
    });

});

