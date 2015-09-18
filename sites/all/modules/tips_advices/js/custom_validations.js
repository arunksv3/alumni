jQuery(document).ready(function($) {
    $("#add-tips-advices").validate({ 
        rules: {            
            category:{
                required:true
            },
            title:{
                required:true
            },
            content_desc:{
                required:true
            },
            embded_code: {
                required:'#edit-article-type-1:checked'
            }            
        },
        messages: {
            category:{
                required: "Please select category"                
            },
            title:{
                required: "Please enter title"                
            },
            content_desc:{
                required: "Please enter description"                
            },
            embded_code:{
                required: "Please enter embded_code"                
            }
        }    
    });  
});

function showOtions(val){ 
    if(val==0){  
        document.getElementById("article_video").style.display = "none";
    }else{
        document.getElementById("article_video").style.display = "block";
    }
}