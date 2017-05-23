var video;
        var display;
        
        window.onload = function(){
            video = document.getElementById("video_player");
            var strGET = window.location.search.replace('?', ''); 
            strGET = strGET.substr(strGET.indexOf("=")+1);
            video.setAttribute("src","library/"+strGET)
        };
        
        function play(){
            video.play();
            video.playbackRate=1;
        };
        
        function pause(){
            video.pause();
        };
        
        function stop(){
            video.pause();
            video.currentTime = 0;
        };
        
        function reverse(){
            video.play();
            switch(video.playbackRate){
                case 1:
                    video.playbackRate = 0.5;
                    break;
                case 0.5:
                    video.playbackRate = 0.25;
                    break;
                case 0.25:
                    video.pause();
                    break;
                default:
                    video.playbackRate = 1;
                    break;
            };
        };
        
        function speed(){
            video.play();
            switch(video.playbackRate){
                case 0:
                    video.playbackRate = 0.25;
                    break;    
                case 0.25:
                    video.playbackRate = 0.5;
                    break;
                case 0.5:
                    video.playbackRate = 1;
                    break;
                case 1:
                    video.playbackRate = 1.5;
                    break;
                case 1.5:
                    video.playbackRate = 2;
                    break;
                case 2:
                    video.playbackRate = 4;
                    break;
                default:
                    video.playbackRate = 1;
                    break;
            };
        };
        
        function full(){
            if (video.requestFullscreen) {
              video.requestFullscreen();
            } else if (video.mozRequestFullScreen) {
              video.mozRequestFullScreen();
            } else if (video.webkitRequestFullscreen) {
              video.webkitRequestFullscreen();
            };
            //disableDefaultUI: true;
        };
        
        function update_time_line(){
            var rem_line = document.getElementById("rem_line");
            rem_line.style.width = (video.currentTime / video.duration * 100) + "%";
            
            var status = document.getElementById("time_status");
            time_status.innerHTML = Math.floor(video.currentTime)+ ":" +Math.floor((video.currentTime-parseInt(video.currentTime)).toFixed(2)*100) + "/" + parseInt(video.duration) + ":" + Math.floor((video.duration-parseInt(video.duration)).toFixed(2)*100);
        };