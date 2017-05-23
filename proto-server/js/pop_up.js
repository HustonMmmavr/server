function hide(nodeId)
            {
                var node = document.getElementById(nodeId);
                if (node)
                {
                    node.style.display = 'none';
                } 
            }

            function show(nodeId)
            {
                var node = document.getElementById(nodeId);
                if (node)
                {
                    node.style.display = 'block';
                } 
            }