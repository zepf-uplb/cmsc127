var pg = require('pg');
var connectionString = process.env.DATABASE_URL || 'postgres://spotify_dba:qpwoeiruty@localhost:5432/spotify';

var client = new pg.Client(connectionString);
client.connect();

query.on('end', function() { client.end(); });
