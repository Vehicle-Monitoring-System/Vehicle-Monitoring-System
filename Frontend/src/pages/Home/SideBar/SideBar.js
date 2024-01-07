import { useEffect, useState, useRef } from 'react';
import axios from 'axios';
import mqtt from 'mqtt';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';

import classNames from 'classnames/bind';
import styles from './SideBar.module.scss';
import { faPaperPlane, faQuestion, faXmark } from '@fortawesome/free-solid-svg-icons';

const cx = classNames.bind(styles);

function SideBar({ listDetail, setListDetail, setCenter }) {
    const [selectedDevices, setSelectedDevices] = useState([]);
    const [listDevice, setListDevice] = useState([]);
    const [mqttConnections, setMqttConnections] = useState([]);

    const selectedDevicesRef = useRef();
    selectedDevicesRef.current = selectedDevices;

    const listDetailRef = useRef();
    listDetailRef.current = listDetail;

    const connectionsRef = useRef();
    connectionsRef.current = mqttConnections;

    var options = {
        username: process.env.REACT_APP_USER_NAME,
        password: process.env.REACT_APP_PASSWORD,
    };

    useEffect(() => {
        const getDevice = async () => {
            const res = await axios.get(`${process.env.REACT_APP_BE_API}/Device`);
            setListDevice(res.data);
        };
        getDevice();

        //Xóa tất cả connect khi unmount
        return () => {
            connectionsRef.current.map((item) => item.client.end());
        };
    }, []);

    const handleChangeDevice = (item) => {
        const isSelected = selectedDevices.includes(item);

        if (isSelected) {
            setSelectedDevices(selectedDevices.filter((selectedItem) => selectedItem !== item));
            const list = listDetail.filter((selectedItem) => selectedItem.DeviceId !== item);
            setListDetail(list);
            leaveTopic(item);
        } else {
            setSelectedDevices([...selectedDevices, item]);
            joinTopic(item);
        }
    };

    const joinTopic = (deviceId) => {
        const client = mqtt.connect(process.env.REACT_APP_MQTT_BROKER, options);

        client.on('connect', () => {
            console.log(`Connected to MQTT broker for topic: ${deviceId}`);
            client.subscribe(`device/${deviceId}`);
        });

        // Lắng nghe sự kiện khi nhận được tin nhắn
        client.on('message', (topic, message) => {
            const byteArray = Array.from(message);
            const jsonString = String.fromCharCode(...byteArray);
            const data = JSON.parse(jsonString);
            console.log(data);
            if (data.Error) {
                if (selectedDevicesRef.current.includes(data.RecentLocation.DeviceId)) {
                    const list = listDetailRef.current.filter((item) => item.DeviceId !== data.RecentLocation.DeviceId);
                    setListDetail([...list, data.RecentLocation]);
                } else {
                    const loseGps = { ...data.RecentLocation, Status: 4 };
                    setListDetail([...listDetailRef.current, loseGps]);
                }
            } else {
                if (selectedDevicesRef.current.includes(data.DeviceId)) {
                    const list = listDetailRef.current.filter((item) => item.DeviceId !== data.DeviceId);
                    setListDetail([...list, data]);
                } else {
                    setListDetail([...listDetailRef.current, data]);
                }
            }
        });

        // Lắng nghe sự kiện khi kết nối bị đóng
        client.on('close', () => {
            console.log(`Connection closed for topic: ${deviceId}`);
        });

        const listConnections = mqttConnections.filter((item) => item.deviceId !== deviceId);
        setMqttConnections([...listConnections, { deviceId: deviceId, client: client }]);
    };

    const leaveTopic = (deviceId) => {
        const removeClient = mqttConnections.filter((item) => item.deviceId === deviceId);
        removeClient[0].client.end();
        const other = mqttConnections.filter((item) => item.deviceId !== deviceId);
        setMqttConnections(other);
    };

    const goToMap = (deviceId) => {
        const seletedMarker = listDetail.filter((tmp) => tmp.DeviceId === deviceId);
        setCenter({ lat: parseFloat(seletedMarker[0].Latitude), lng: parseFloat(seletedMarker[0].Longitude) });
    };

    return (
        <div className={cx('wrapper')}>
            <div>
                <h2>Giám sát trực tuyến</h2>
                <div className={cx('line')}></div>
                <h4>Danh sách các xe:</h4>
                {listDevice.map((item, index) => (
                    <div key={index} className={cx('device_item')}>
                        <input
                            type="checkbox"
                            id={index}
                            checked={selectedDevices.includes(item.deviceId)}
                            onChange={() => handleChangeDevice(item.deviceId)}
                        />
                        <label htmlFor={index}>{item.deviceLicensePlates}</label>
                        <div className={cx('icon')}>
                            {selectedDevices.includes(item.deviceId) ? (
                                listDetail.filter((tmp) => tmp.DeviceId === item.deviceId).length > 0 ? (
                                    <FontAwesomeIcon icon={faPaperPlane} onClick={() => goToMap(item.deviceId)} />
                                ) : (
                                    <FontAwesomeIcon icon={faXmark} />
                                )
                            ) : (
                                <FontAwesomeIcon icon={faQuestion} />
                            )}
                        </div>
                    </div>
                ))}
            </div>
        </div>
    );
}

export default SideBar;
