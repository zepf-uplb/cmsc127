var express = require('express');
var router = express.Router();
var path = require('path');
var pg = require('pg');
var connectionString = process.env.DATABASE_URL || 'postgres://spotify_dba:qpwoeiruty@localhost:5432/spotify';

/* GET home page. */
router.get('/', function(req, res, next) {
  res.sendfile(path.join(__dirname, '../views', 'index.html'));
});

router.get('/sign-up', function(req, res, next) {
  res.sendfile(path.join(__dirname, '../views', 'sign-up.html'));
});

router.get('/log-in', function(req, res, next) {
  res.sendfile(path.join(__dirname, '../views', 'log-in.html'));
});

router.get('/success_sign-up', function(req, res, next) {
  res.sendfile(path.join(__dirname, '../views', 'success_sign-up.html'));
});

router.get('/admin_sign-up', function(req, res, next) {
  res.sendfile(path.join(__dirname, '../views', 'admin_sign-up.html'));
});

router.get('/admin_log-in', function(req, res, next) {
  res.sendfile(path.join(__dirname, '../views', 'admin_log-in.html'));
});

router.get('/profile', function(req, res, next) {  
  res.sendfile(path.join(__dirname, '../views', 'profile.html'));
});

router.get('/playlist', function(req, res, next) {  
  res.sendfile(path.join(__dirname, '../views', 'playlist.html'));
});
/*========================================================*/
router.post('/qpwoeirutuy/users', function(req, res) {

    var results = [];

    // Grab data from http request
    var data = {username: req.body.username, password: req.body.password, email: req.body.email, birthday: req.body.birthday, gender: req.body.gender};

    // Get a Postgres client from the connection pool
    pg.connect(connectionString, function(err, client, done) {
        // Handle connection errors
        if(err) {
          done();
          console.log(err);
          return res.status(500).json({ success: false, data: err});
        }
        // SQL Query > Insert Data
        client.query("INSERT INTO user_(username, password, email_address, birthday, gender) VALUES($1, $2, $3, $4, $5)", [data.username, data.password, data.email, data.birthday, data.gender]);

        done();
        return res.json(results);
    });
});
/*========================================================*/
router.post('/qpwoeirutuya/users', function(req, res) {

    var results = [];

    // Grab data from http request
    var data = {username: req.body.username, password: req.body.password};

    // Get a Postgres client from the connection pool
    pg.connect(connectionString, function(err, client, done) {
        // Handle connection errors
        if(err) {
          done();
          console.log(err);
          return res.status(500).json({ success: false, data: err});
        }
        // SQL Query > Insert Data
        var query = client.query("SELECT * FROM user_ where username=$1 AND password=$2", [data.username, data.password]);

        // Stream results back one row at a time
        query.on('row', function(row) {
            results.push(row);
        });

        // After all data is returned, close connection and return results
        query.on('end', function() {
            done();
            return res.json(results);
        });
    });
});
/*========================================================*/
router.post('/qpwoeirutuy/admins', function(req, res) {

    var results = [];

    // Grab data from http request
    var data = {username: req.body.username, password: req.body.password, email: req.body.email, gender: req.body.gender};

    // Get a Postgres client from the connection pool
    pg.connect(connectionString, function(err, client, done) {
        // Handle connection errors
        if(err) {
          done();
          console.log(err);
          return res.status(500).json({ success: false, data: err});
        }
        // SQL Query > Insert Data
        client.query("INSERT INTO administrator VALUES($1, $2, $3, $4)", [data.username, data.password, data.email, data.gender]);

        done();
        return res.json(results);
    });
});
/*========================================================*/
router.post('/qpwoeirutuya/admins', function(req, res) {

    var results = [];

    // Grab data from http request
    var data = {username: req.body.username, password: req.body.password};

    // Get a Postgres client from the connection pool
    pg.connect(connectionString, function(err, client, done) {
        // Handle connection errors
        if(err) {
          done();
          console.log(err);
          return res.status(500).json({ success: false, data: err});
        }
        // SQL Query > Insert Data
        var query = client.query("SELECT * FROM administrator where admin_username=$1 AND admin_password=$2", [data.username, data.password]);

        // Stream results back one row at a time
        query.on('row', function(row) {
            results.push(row);
        });

        // After all data is returned, close connection and return results
        query.on('end', function() {
            done();
            return res.json(results);
        });
    });
});
/*========================================================*/
router.post('/qpwoeirutuy/playlists', function(req, res) {

    var results = [];

    // Grab data from http request
    var data = {email: req.body.email_address};

    // Get a Postgres client from the connection pool
    pg.connect(connectionString, function(err, client, done) {
        // Handle connection errors
        if(err) {
          done();
          console.log(err);
          return res.status(500).json({ success: false, data: err});
        }
        // SQL Query > Insert Data
        var query = client.query("INSERT INTO playlist(email_address) VALUES($1)", [data.email]);


      // SQL Query > Select Data
      var query = client.query("SELECT * FROM playlist WHERE email_address = $1", [data.email]);

        // Stream results back one row at a time
        query.on('row', function(row) {
            results.push(row);
        });

        // After all data is returned, close connection and return results
        query.on('end', function() {
            done();
            return res.json(results);
        });
    });
});
/*========================================================*/
router.post('/qpwoeirutuy/playlistss', function(req, res) {

    var results = [];

    // Grab data from http request
    var data = {email: req.body.email_address};
    //console.log(data.email);

    // Get a Postgres client from the connection pool
    pg.connect(connectionString, function(err, client, done) {
        // Handle connection errors
        if(err) {
          done();
          console.log(err);
          return res.status(500).json({ success: false, data: err});
        }

      // SQL Query > Select Data
      var query = client.query("SELECT * FROM playlist WHERE email_address = $1", [data.email]);

        // Stream results back one row at a time
        query.on('row', function(row) {
            results.push(row);
        });

        // After all data is returned, close connection and return results
        query.on('end', function() {
            done();
            return res.json(results);
        });
    });
});
/*========================================================*/
router.delete('/qpwoeirutuy/playlists/:list_id', function(req, res) {

    var results = [];

    // Grab data from the URL parameters
    var id = req.params.list_id;
    console.log(id);

    // Get a Postgres client from the connection pool
    pg.connect(connectionString, function(err, client, done) {
        // Handle connection errors
        if(err) {
          done();
          console.log(err);
          return res.status(500).json({ success: false, data: err});
        }

        // SQL Query > Insert Data
        var query = client.query("DELETE FROM playlist WHERE playlist_id = ($1)", [id]);

      // SQL Query > Select Data
      var query = client.query("SELECT * FROM playlist;");
      
        // Stream results back one row at a time
        query.on('row', function(row) {
            results.push(row);
        });

        // After all data is returned, close connection and return results
        query.on('end', function() {
            done();
            return res.json(results);
        });
    });
});
/*========================================================*/
router.get('/qpwoeirutuy/playlists/:list_id', function(req, res) {

    var results = [];

    // Grab data from the URL parameters
    var id = req.params.list_id;

    // Get a Postgres client from the connection pool
    pg.connect(connectionString, function(err, client, done) {
        // Handle connection errors
        if(err) {
          done();
          console.log(err);
          return res.status(500).json({ success: false, data: err});
        }

        // SQL Query > Insert Data
        var query = client.query("SELECT * FROM contain WHERE playlist_id = ($1)", [id]);

       // Stream results back one row at a time
        query.on('row', function(row) {
            results.push(row);
        });

        // After all data is returned, close connection and return results
        query.on('end', function() {
            done();
            return res.json(results);
        });
    });
});
/*========================================================*/
module.exports = router;
