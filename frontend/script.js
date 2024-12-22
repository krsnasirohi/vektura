async function initMap() {

  let fetchRes = await fetch("https://<PLACEHOLDER FOR API GATEWAY URL>/prod/getloc?routeId=<PLACE HOLDER FOR ROUTE ID>");
  let res = await fetchRes.json();
  console.log(res);

  const map = new google.maps.Map(document.getElementById("map"), {
    zoom: 15,
    center: {lat: parseFloat(res.lat), lng: parseFloat(res.longitude)},
    mapTypeId: "terrain",
  });
  
  const marker0 = new google.maps.Marker({
    position: {lat: 12.972071193885165, lng: 77.62060371842915},
    map: map,
    label: {
    	color: 'black',
      fontWeight: 'bold',
      text: 'Trinty Church',
    },
  });

  const marker1 = new google.maps.Marker({
    position: { lat: 12.974053085791482, lng: 77.61067241949775},
    map: map,
    label: {
    	color: 'black',
      fontWeight: 'bold',
      text: 'Mayo Hall',
    },
  });

  const marker2 = new google.maps.Marker({
    position: { lat: 12.976592629842639, lng: 77.5993324251676},
    map: map,
    label: {
    	color: 'black',
      fontWeight: 'bold',
      text: 'Namma Bengaluru Aquarium',
    },
  });

  const marker3 = new google.maps.Marker({
    position: { lat: 12.975460133681214, lng: 77.59665590488821},
    map: map,
    label: {
    	color: 'black',
      fontWeight: 'bold',
      text: 'Visvesvaraya Museum',
    },
  });

  const busRouteCoordinates = [
    {lat: 12.972071193885165, lng: 77.62060371842915},
    {lat: 12.972268525782615, lng: 77.62028676213161},
    {lat: 12.972268525782615, lng: 77.61965284943386},
    {lat: 12.97141055970102, lng: 77.61954719731757},
    {lat: 12.972508755755122, lng: 77.6191950235966},
    {lat: 12.974361950600851, lng: 77.61095415852586},
    {lat: 12.976034600853392, lng: 77.60484943861115},
    {lat: 12.97632733690276, lng: 77.60274658690962},
    {lat: 12.976452795104114, lng: 77.59918461361929},
    {lat: 12.9755327668241, lng: 77.59995708975454},
    {lat: 12.973316321080523, lng: 77.59944210566438},
    {lat: 12.971016214989673, lng: 77.59781132271218},
    {lat: 12.971434417679157, lng: 77.59600887839657},
    {lat: 12.971978080124346, lng: 77.59412060339928},
    {lat: 12.973316321080523, lng: 77.59506474089793},
    {lat: 12.974529095727206, lng: 77.59669552385013}
  ];
  
 const busRoute = new google.maps.Polyline({
    path: busRouteCoordinates,
    geodesic: true,
    strokeColor: "#4285F4",
    strokeOpacity: 1.0,
    strokeWeight: 9,
  });
    
  const cityCircle = new google.maps.Circle({
    strokeColor: "#FF0000",
    strokeOpacity: 1,
    strokeWeight: 9,
    fillColor: "#FF0000",
    fillOpacity: 0.20,
    map,
    center: { lat: parseFloat(res.lat), lng: parseFloat(res.longitude)},
    radius: 20,
  });


  busRoute.setMap(map);
  


  setInterval( async function(){
    let fetchRes = await fetch("https://<PLACEHOLDER FOR API GATEWAY URL>/prod/getloc?routeId=<PLACE HOLDER FOR ROUTE ID>");
    let res = await fetchRes.json();
    console.log(res);

    cityCircle.setCenter(new google.maps.LatLng(parseFloat(res.lat),parseFloat(res.longitude)));
    map.setCenter(new google.maps.LatLng(parseFloat(res.lat),parseFloat(res.longitude)));  
  }, 30000);
  
}

initMap();



