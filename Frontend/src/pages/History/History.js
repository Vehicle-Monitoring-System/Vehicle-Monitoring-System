import { GoogleMap, useJsApiLoader, Polyline, Marker, InfoWindow } from '@react-google-maps/api';
import { useState } from 'react';
import classNames from 'classnames/bind';
import moment from 'moment';

import SideBarHis from './SideBarHis';
import styles from './History.module.scss';

const cx = classNames.bind(styles);

function History() {
    const [listPoly, setListPoly] = useState([]);
    const [isMarkerVisible, setIsMarkerVisible] = useState(true);
    const [showInfo, setShowInfo] = useState({ time: '', show: false });

    const [center, setCenter] = useState({
        lat: parseFloat('21.004494'),
        lng: parseFloat('105.846466'),
    });

    const { isLoaded } = useJsApiLoader({
        googleMapsApiKey: process.env.REACT_APP_GOOGLE_MAPS_API_KEY,
    });

    const handleMarkerClick = (time) => {
        if (showInfo.time === time) {
            setShowInfo({ ...showInfo, show: !showInfo.show });
        } else {
            setShowInfo({ time: time, show: true });
        }
    };

    const checkSpeed = (speed) => {
        const convertedSpeed = (speed * 1.8).toFixed(2);
        return `${convertedSpeed} km/h`;
    };

    const checkStatus = (status) => {
        switch (status) {
            case 1:
                return ' Xe đang di chuyển';
            case 2:
                return ' Xe đang dừng';
            case 3:
                return ' Xe đỗ';
            default:
                return ' Mất GPS';
        }
    };

    const checkTime = (time) => {
        const formatTime = moment(time).format('h:mm:ss a DD/MM/YYYY');
        return formatTime;
    };

    const checkLocationInfo = (data) => {
        return `${data} dB`;
    };

    const renderPolylines = () => {
        const arrowPath = [];
        const polylines = [];

        for (let i = 0; i < listPoly.length; i++) {
            const path = {
                lat: parseFloat(listPoly[i].latitude),
                lng: parseFloat(listPoly[i].longitude),
            };
            arrowPath.push(path);

            console.log(listPoly[i]);

            const marker = (
                <Marker
                    key={listPoly[i].time}
                    position={path}
                    visible={isMarkerVisible}
                    onClick={() => handleMarkerClick(listPoly[i].time)}
                >
                    {showInfo.show && showInfo.time === listPoly[i].time && (
                        <InfoWindow onCloseClick={() => handleMarkerClick(listPoly[i].time)}>
                            <div className={cx('info-box')}>
                                <p>Biển số xe: {listPoly[i].deviceLicensePlates}</p>
                                <p>Tên thiết bị: {listPoly[i].deviceName}</p>
                                <p>Trạng thái thiết bị:{checkStatus(listPoly[i].status)}</p>
                                <p>Vận tốc: {checkSpeed(listPoly[i].speed)}</p>
                                <p>Sóng: {checkLocationInfo(listPoly[i].locationInfo)}</p>
                                <p>Thời gian: {checkTime(listPoly[i].time)}</p>
                                <p>Tên người lái: {listPoly[i].driverName}</p>
                                <p>Số hiệu bằng lái xe người lái: {listPoly[i].driverLicense}</p>
                            </div>
                        </InfoWindow>
                    )}
                </Marker>
            );

            polylines.push(marker);
        }

        const polyline = (
            <Polyline
                key={1}
                path={arrowPath}
                options={{
                    strokeColor: '#FF0000',
                    strokeOpacity: 1.0,
                    strokeWeight: 2,
                    icons: [
                        {
                            icon: {
                                path: window.google.maps.SymbolPath.FORWARD_CLOSED_ARROW,
                            },
                            offset: '50%',
                            repeat: '100px',
                            fillColor: 'blue',
                        },
                    ],
                }}
            />
        );

        polylines.push(polyline);

        return polylines;
    };

    if (!isLoaded) {
        return <h2>Loading</h2>;
    }

    return (
        <div className={cx('container')}>
            <SideBarHis setListPoly={setListPoly} setCenter={setCenter} />
            <div className={cx('content')}>
                <GoogleMap
                    center={center}
                    zoom={16}
                    mapContainerStyle={{ width: '100%', height: '100%', borderRadius: '16px' }}
                    options={{
                        zoomControl: false,
                        streetViewControl: false,
                        fullscreenControl: false,
                        mapTypeControl: false,
                    }}
                >
                    {renderPolylines()}
                    <button
                        onClick={() => setIsMarkerVisible(!isMarkerVisible)}
                        style={{
                            position: 'absolute',
                            top: '10px',
                            right: '10px',
                            zIndex: 1,
                            padding: '10px',
                        }}
                    >
                        {isMarkerVisible ? 'Tắt các điểm' : 'Bật các điểm'}
                    </button>
                </GoogleMap>
            </div>
        </div>
    );
}

export default History;
