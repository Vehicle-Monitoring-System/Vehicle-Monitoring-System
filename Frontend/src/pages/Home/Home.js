import { GoogleMap, useJsApiLoader, Marker, InfoWindow } from '@react-google-maps/api';
import { useState } from 'react';
import classNames from 'classnames/bind';
import moment from 'moment';

import Sidebar from './SideBar';
import styles from './Home.module.scss';
import images from '../../assets/images';

const cx = classNames.bind(styles);

function Home() {
    const [listDetail, setListDetail] = useState([]);
    const [showInfo, setShowInfo] = useState({ id: '', show: false });
    const [center, setCenter] = useState({ lat: parseFloat('21.004494'), lng: parseFloat('105.846466') });

    const { isLoaded } = useJsApiLoader({
        googleMapsApiKey: process.env.REACT_APP_GOOGLE_MAPS_API_KEY,
    });

    const handleMarkerClick = (deviceId) => {
        if (showInfo.id === deviceId) {
            setShowInfo({ ...showInfo, show: !showInfo.show });
        } else {
            setShowInfo({ id: deviceId, show: true });
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

    const loadMarker = (status) => {
        switch (status) {
            case 1:
                return images.car1;
            case 2:
                return images.car2;
            case 3:
                return images.car2;
            default:
                return images.car3;
        }
    };

    if (!isLoaded) {
        return <h2>Loading</h2>;
    }

    return (
        <div className={cx('container')}>
            <Sidebar listDetail={listDetail} setListDetail={setListDetail} setCenter={setCenter} />
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
                    {listDetail?.map((item) => (
                        <Marker
                            key={item.DeviceId}
                            icon={{
                                url: loadMarker(item.Status),
                                scaledSize: new window.google.maps.Size(40, 40),
                                origin: new window.google.maps.Point(0, 0),
                                anchor: new window.google.maps.Point(20, 40),
                            }}
                            position={{ lat: parseFloat(item.Latitude), lng: parseFloat(item.Longitude) }}
                            onClick={() => handleMarkerClick(item.DeviceId)}
                        >
                            {showInfo.show && showInfo.id === item.DeviceId && (
                                <InfoWindow onCloseClick={() => handleMarkerClick(item.DeviceId)}>
                                    <div className={cx('info-box')}>
                                        <p>Biển số xe: {item.DeviceLicensePlates}</p>
                                        <p>Tên thiết bị: {item.DeviceName}</p>
                                        <p>Trạng thái thiết bị:{checkStatus(item.Status)}</p>
                                        <p>Vận tốc: {checkSpeed(item.Speed)}</p>
                                        <p>Sóng: {checkLocationInfo(item.LocationInfo)}</p>
                                        <p>Thời gian: {checkTime(item.Time)}</p>
                                        <p>Tên người lái: {item.DriverName}</p>
                                        <p>Số hiệu bằng lái xe người lái: {item.DriverLicense}</p>
                                    </div>
                                </InfoWindow>
                            )}
                        </Marker>
                    ))}
                </GoogleMap>
            </div>
        </div>
    );
}

export default Home;
