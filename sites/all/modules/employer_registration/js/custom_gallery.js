function del(){
    var cnf = confirm('Are you sure you want to delete this image? please click Ok to delete.');
    if(cnf)
        return true;
    else
        return false;    
}

jQuery(document).ready(function($) {
    $('.popup-link').magnificPopup({
        type:'image'
    });
});
