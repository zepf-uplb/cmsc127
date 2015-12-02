angular.module('spotifyApp', [])

.controller('mainController', function($scope, $http) {

$scope.signupData = {};
$scope.loginData = {};
$scope.user = {};
$scope.userData = {};
$scope.playlistData = {};
$scope.musicData = {};

$scope.addUser = function() {
    $http.post('/qpwoeirutuy/users', $scope.signupData)
        .success(function(data) {
            $scope.signupData = {};
        });
}

$scope.loginUser = function() {
    $http.post('/qpwoeirutuya/users', $scope.loginData)
        .success(function(data) {
            document.cookie = data[0].email_address;
            console.log(data[0].email_address);
        });
}

$scope.addAdmin = function() {
    $http.post('/qpwoeirutuy/admins', $scope.signupData)
        .success(function(data) {
            $scope.signupData = {};
        });
}

$scope.loginAdmin = function() {
    $http.post('/qpwoeirutuya/admins', $scope.loginData)
        .success(function(data) {
           document.cookie = data[0].email_address;
        });
}

$scope.getPlaylistData = function(){
    $scope.userData.email_address = document.cookie;
    $http.post('/qpwoeirutuy/playlistss', $scope.userData)
    .success(function(data) {
        $scope.playlistData = data;
    });
}

$scope.addPlaylist = function() {
    $scope.userData.email_address = document.cookie;
    $http.post('/qpwoeirutuy/playlists', $scope.userData)
        .success(function(data) {
            $scope.playlistData = data;
        });
}

$scope.removePlaylist = function(listID) {
    $http.delete('/qpwoeirutuy/playlists/' + listID)
        .success(function(data) {
            $scope.playlistData = data;
        });
}

$scope.goToPlaylist = function(listID) {
    $http.get('/qpwoeirutuy/playlists/' + listID)
        .success(function(data) {
            $scope.musicData = data;
            $scope.playlistData.playlist_id = listID;
        });
}

$scope.userLogout = function() {
    document.cookie = "";
}

});


